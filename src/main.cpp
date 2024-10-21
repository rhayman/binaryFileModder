#include "indicators/setting.hpp"
#include <argparse/argparse.hpp>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <indicators/block_progress_bar.hpp>
#include <indicators/cursor_control.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;
namespace ind = indicators;

void print_vector(std::vector<int16_t> &vec) {
  for (auto v : vec)
    std::cout << v << "\t";
  std::cout << std::endl;
}

std::vector<int16_t> remove_rows(std::vector<int16_t> vec,
                                 int n_rows_to_remove) {
  std::vector<int16_t> out(vec.begin() + 1, vec.end() - n_rows_to_remove);
  return out;
}

std::vector<int16_t> add_rows(std::vector<int16_t> vec, int n_elems_to_add,
                              int fill_value) {
  std::vector<int16_t> b(n_elems_to_add, fill_value);
  vec.insert(std::end(vec), std::begin(b), std::end(b));
  return vec;
}

bool add_columns(fs::path in, fs::path out, int n_columns_to_add,
                 int fill_value, int n_channels) {
  // copy the file to a new location first
  auto result = fs::copy_file(in, out, fs::copy_options::overwrite_existing);
  // now open out to append columns
  // create the column first
  std::vector<int16_t> column(n_channels, fill_value);
  std::fstream strm(out, std::ios::in | std::ios::out | std::ios::binary);
  if (!strm.is_open()) {
    std::cout << "Failed to open: " << out << std::endl;
    return false;
  } else {
    // go to end of file
    strm.seekg(0, std::ios::end);
    for (int i = 0; i < n_columns_to_add; ++i) {
      strm.write((char *)&column[0], column.size() * sizeof(int16_t));
    }
  }
  return true;
}

int main(int argc, char *argv[]) {

  argparse::ArgumentParser program("binaryFileModder", "0.0.1");

  std::string input_file;
  program.add_argument("-i", "--input")
      .required()
      .help("The input file name")
      .store_into(input_file);

  std::string output_file = "output.dat";
  program.add_argument("-o", "--output")
      .help("The output file name (default=output.dat)")
      .store_into(output_file);

  int n_channels = 64;
  program.add_argument("-n", "--n_channels")
      .help("The number of channels in the input file (default=64)")
      .store_into(n_channels);

  int16_t fill_value = 0;
  program.add_argument("-f", "--fill_value")
      .help("The fill value for adding data file name (default=0)")
      .store_into(fill_value);

  int add_n_rows = 0;
  program.add_argument("-aR", "--add_rows")
      .help("The number of rows to add")
      .store_into(add_n_rows);

  int remove_n_rows = 0;
  program.add_argument("-rR", "--remove_rows")
      .help("The number of rows to remove")
      .store_into(remove_n_rows);

  int add_n_columns = 0;
  program.add_argument("-aC", "--add_columns")
      .help("The number of columns to add")
      .store_into(add_n_columns);

  int remove_n_columns = 0;
  program.add_argument("-rC", "--remove_columns")
      .help("The number of columns to remove")
      .store_into(remove_n_columns);

  try {
    program.parse_args(argc, argv);
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    return 1;
  }

  // indicators stuff
  ind::show_console_cursor(false);

  ind::BlockProgressBar bar{ind::option::BarWidth{80},
                            ind::option::Start{"["},
                            ind::option::End{"]"},
                            ind::option::ShowElapsedTime(true),
                            ind::option::ShowRemainingTime(true),
                            ind::option::ForegroundColor{ind::Color::white},
                            ind::option::FontStyles{std::vector<ind::FontStyle>{
                                ind::FontStyle::bold}}};

  // fs::path full_path(fs::initial_path());
  // check the input file exists
  fs::path input_file_path(input_file);

  if (!fs::exists(input_file_path)) {
    std::cout << "File " << input_file_path << " does not exist" << std::endl;
    return 1;
  }
  auto n_cols = fs::file_size(input_file_path) / n_channels / sizeof(int16_t);
  // input file stream
  std::ifstream fin(input_file_path, std::ifstream::binary);
  // output file stream
  fs::path output_file_path(output_file);
  std::ofstream fout(output_file_path, std::ofstream::binary);

  std::vector<int16_t> buffer(n_channels, 0);
  double column_count = 0;
  if (add_n_columns > 0 || add_n_rows > 0 || remove_n_columns > 0 ||
      remove_n_rows > 0) {
    while (fin.read(reinterpret_cast<char *>(buffer.data()),
                    n_channels * sizeof(int16_t))) {
      auto count = fin.gcount();
      if (add_n_rows > 0) {
        auto out_buf = add_rows(buffer, add_n_rows, fill_value);
        fout.write((char *)&out_buf[0], out_buf.size() * sizeof(int16_t));
      }
      if (remove_n_rows > 0) {
        auto out_buf = remove_rows(buffer, remove_n_rows);
        fout.write((char *)&out_buf[0], out_buf.size() * sizeof(int16_t));
      }
      if (add_n_columns > 0) {
        // can do faster using memcpy then appending to end of copy
        // so done last of all - this will copy the original file NOT
        // the new one so if you want to add columns AND remove/add rows
        // then you'd need to call this program twice, once for each
        // operation
        add_columns(input_file_path, output_file_path, add_n_columns,
                    fill_value, n_channels);
        bar.set_progress(100);
        break;
      }
      if (remove_n_columns > 0) {
        if (column_count == (n_cols - remove_n_columns)) {
          bar.set_progress(100);
          break;
        }
        fout.write((char *)&buffer[0], buffer.size() * sizeof(int16_t));
      }
      ++column_count;
      bar.set_progress((column_count / n_cols) * 100);
    }
  }
  if (fin.is_open()) {
    fin.close();
  }
  if (fout.is_open()) {
    fout.close();
  }
  ind::show_console_cursor(true);

  if (add_n_columns > 0 || add_n_rows > 0 || remove_n_columns > 0 ||
      remove_n_rows > 0) {
    std::cout << "\nOutput file: " << fs::canonical(output_file_path)
              << std::endl;
  }
  return 0;
}

#include <argparse/argparse.hpp>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

void print_vector(std::vector<int16_t> &vec) {
  for (auto v : vec)
    std::cout << v << "\t";
  std::cout << std::endl;
}

std::vector<int16_t> truncate_buffer(std::vector<int16_t> vec,
                                     int n_rows_to_remove) {
  std::vector<int16_t> out(vec.begin() + 1, vec.end() - n_rows_to_remove);
  return out;
}

std::vector<int16_t> extend_buffer(std::vector<int16_t> vec, int n_elems_to_add,
                                   int fill_value) {
  std::vector<int16_t> b(n_elems_to_add, fill_value);
  vec.insert(std::end(vec), std::begin(b), std::end(b));
  return vec;
}

int main(int argc, char *argv[]) {

  argparse::ArgumentParser program("program_name");

  std::string input_file;
  program.add_argument("-i", "--input")
      .required()
      .help("The input file name")
      .store_into(input_file);

  std::string output_file = "output.dat";
  program.add_argument("-o", "--output")
      .help("The output file name")
      .store_into(output_file);

  int n_channels = 64;
  program.add_argument("-n", "--n_channels")
      .help("The number of channels in the input file")
      .store_into(n_channels);

  int16_t fill_value = 0;
  program.add_argument("-f", "--fill_value")
      .help("The fill value for adding data file name")
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

  // fs::path full_path(fs::initial_path());
  // check the input file exists
  fs::path input_file_path(input_file);

  if (!fs::exists(input_file_path)) {
    std::cout << "File " << input_file_path.native() << " does not exist"
              << std::endl;
    return 1;
  }
  // input file stream
  std::ifstream fin(input_file_path, std::ifstream::binary);
  // output file stream
  fs::path output_file_path(output_file);
  std::ofstream fout(output_file_path, std::ofstream::binary);

  std::vector<int16_t> buffer(n_channels, 0);
  int column_count = 0;
  while (fin.read(reinterpret_cast<char *>(buffer.data()),
                  n_channels * sizeof(int16_t))) {
    auto count = fin.gcount();
    if (add_n_rows > 0) {
      auto out_buf = extend_buffer(buffer, add_n_rows, fill_value);
      fout.write((char *)&out_buf[0], out_buf.size() * sizeof(int16_t));
    }
    if (remove_n_rows) {
      auto out_buf = truncate_buffer(buffer, remove_n_rows);
      fout.write((char *)&out_buf[0], out_buf.size() * sizeof(int16_t));
    }
    ++column_count;
  }
  if (fin.is_open()) {
    fin.close();
  }
  if (fout.is_open()) {
    fout.close();
  }
  return 0;
}

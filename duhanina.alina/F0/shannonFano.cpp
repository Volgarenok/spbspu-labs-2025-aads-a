#include "shannonFano.hpp"
#include <fstream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <list.hpp>
#include <array.hpp>

namespace
{
  duhanina::Tree< std::string, duhanina::CodeTable, std::less< std::string > > encoding_store;

  using str_t = const std::string&;

  const std::string TEXT_EXT = ".txt";
  const std::string COMPRESSED_EXT = ".sfano";
  const std::string CODE_TABLE_EXT = ".sfcodes";

  void validate_extension(str_t filename, str_t expected_ext)
  {
    size_t dot_pos = filename.find_last_of('.');
    if (dot_pos == std::string::npos || filename.substr(dot_pos) != expected_ext)
    {
      throw std::runtime_error("Invalid file extension. Expected: " + expected_ext);
    }
  }

  void build_shannon_fano_codes(duhanina::NodeSymb* node, std::string code, duhanina::Tree< char, std::string, std::less< char > >& codes)
  {
    if (!node)
    {
      return;
    }
    if (node->left == nullptr && node->right == nullptr)
    {
      codes[node->symbol] = code;
      return;
    }
    if (node->left != nullptr)
    {
      build_shannon_fano_codes(node->left, code + "0", codes);
    }
    if (node->right != nullptr)
    {
      build_shannon_fano_codes(node->right, code + "1", codes);
    }
  }

  void delete_tree(duhanina::NodeSymb* node)
  {
    if (node != nullptr)
    {
      delete_tree(node->left);
      delete_tree(node->right);
      delete node;
    }
  }

  duhanina::NodeSymb* build_subtree(const duhanina::DynamicArray< duhanina::NodeSymb* >& nodes, size_t start, size_t end)
  {
    if (start >= end || nodes.empty() || start >= nodes.size())
    {
      return nullptr;
    }
    duhanina::NodeSymb* subtree = nodes[start];
    for (size_t i = start + 1; i < end; i++)
    {
      duhanina::NodeSymb* new_node = nullptr;
      try
      {
        new_node = new duhanina::NodeSymb(subtree->freq + nodes[i]->freq, subtree, nodes[i]);
      }
      catch (...)
      {
        delete_tree(subtree);
        throw std::runtime_error("Error memory");
      }
      subtree = new_node;
    }
    return subtree;
  }

  std::pair< duhanina::NodeSymb*, duhanina::NodeSymb* > split_nodes(const duhanina::DynamicArray< duhanina::NodeSymb* >& nodes)
  {
    size_t total = 0;
    for (size_t i = 0; i < nodes.size(); ++i)
    {
      total += nodes[i]->freq;
    }
    size_t sum = 0;
    size_t split_pos = 0;
    for (; split_pos < nodes.size(); split_pos++)
    {
      sum += nodes[split_pos]->freq;
      if (sum >= total / 2)
      {
        break;
      }
    }
    duhanina::NodeSymb* left = build_subtree(nodes, 0, split_pos);
    duhanina::NodeSymb* right = build_subtree(nodes, split_pos + 1, nodes.size());
    return { left, right };
  }

  duhanina::DynamicArray< duhanina::NodeSymb* > merge_nodes(duhanina::NodeSymb* left, duhanina::NodeSymb* right)
  {
    duhanina::DynamicArray< duhanina::NodeSymb* > new_nodes;
    if (left)
    {
      new_nodes.push_back(left);
    }
    if (right)
    {
      new_nodes.push_back(right);
    }
    return new_nodes;
  }

  duhanina::CodeTable build_code_table(str_t text)
  {
    if (text.empty())
    {
      throw std::runtime_error("EMPTY");
    }
    duhanina::Tree< char, size_t, std::less< char > > freq_map;
    for (size_t i = 0; i < text.size(); i++)
    {
      char c = text[i];
      freq_map[c]++;
    }
    if (freq_map.size() == 1)
    {
      throw std::runtime_error("SINGLE_SYMBOL");
    }
    duhanina::DynamicArray< std::pair< char, size_t > > sorted_freq;
    for (auto it = freq_map.begin(); it != freq_map.end(); ++it)
    {
      sorted_freq.push_back(*it);
    }
    for (size_t i = 0; i < sorted_freq.size(); i++)
    {
      for (size_t j = i + 1; j < sorted_freq.size(); j++)
      {
        if (sorted_freq[i].second < sorted_freq[j].second)
        {
          std::swap(sorted_freq[i], sorted_freq[j]);
        }
      }
    }
    duhanina::DynamicArray< duhanina::NodeSymb* > nodes;
    try
    {
      for (size_t i = 0; i < sorted_freq.size(); i++)
      {
        char ch = sorted_freq[i].first;
        size_t freq = sorted_freq[i].second;
        nodes.push_back(new duhanina::NodeSymb(ch, freq));
      }
    }
    catch (...)
    {
      for (size_t i = 0; i < nodes.size(); i++)
      {
        delete nodes[i];
        nodes[i] = nullptr;
      }
      throw std::runtime_error("Error memory");
    }
    while (nodes.size() > 1)
    {
      auto node_pair = split_nodes(nodes);
      nodes = merge_nodes(node_pair.first, node_pair.second);
    }
    duhanina::CodeTable table;
    table.total_chars = text.size();
    build_shannon_fano_codes(nodes[0], "", table.char_to_code);
    for (auto it = table.char_to_code.begin(); it != table.char_to_code.end(); ++it)
    {
      char ch = it->first;
      std::string code = it->second;
      table.code_to_char[code] = ch;
    }
    delete_tree(nodes[0]);
    return table;
  }

  std::string encode_text(str_t text, const duhanina::CodeTable& table)
  {
    std::string encoded;
    for (size_t i = 0; i < text.size(); i++)
    {
      char c = text[i];
      auto it = table.char_to_code.find(c);
      if (it == table.char_to_code.cend())
      {
        throw std::runtime_error("INVALID_CODES");
      }
      encoded += it->second;
    }
    return encoded;
  }

  std::string decode_text(str_t encoded, const duhanina::CodeTable& table)
  {
    std::string decoded;
    std::string current_code;
    for (size_t i = 0; i < encoded.size(); i++)
    {
      char bit = encoded[i];
      current_code += bit;
      auto it = table.code_to_char.find(current_code);
      if (it != table.code_to_char.cend())
      {
        decoded += it->second;
        current_code.clear();
      }
    }
    if (!current_code.empty())
    {
      throw std::runtime_error("INVALID_CODES");
    }
    return decoded;
  }

  void write_bits_to_file(str_t bits, str_t filename)
  {
    std::ofstream out(filename, std::ios::binary);
    if (!out)
    {
      throw std::runtime_error("INVALID_FILE");
    }
    size_t bit_count = bits.size();
    for (size_t i = 0; i < sizeof(size_t); i++)
    {
      char byte = (bit_count >> (8 * i)) & 0xFF;
      out.put(byte);
    }
    char buffer = 0;
    size_t bit_pos = 0;
    for (size_t i = 0; i < bits.size(); i++)
    {
      if (bits[i] == '1')
      {
        buffer |= (1 << (7 - bit_pos));
      }
      bit_pos++;
      if (bit_pos == 8)
      {
        out.put(buffer);
        buffer = 0;
        bit_pos = 0;
      }
    }
    if (bit_pos > 0)
    {
      out.put(buffer);
    }
  }

  std::string read_bits_from_file(str_t filename)
  {
    std::ifstream in(filename, std::ios::binary);
    if (!in)
    {
      throw std::runtime_error("FILE_NOT_FOUND");
    }
    size_t bit_count = 0;
    for (size_t i = 0; i < sizeof(size_t); i++)
    {
      char byte;
      if (!in.get(byte))
      {
        throw std::runtime_error("INVALID_HEADER");
      }
      bit_count |= static_cast< size_t >(static_cast< unsigned char >(byte)) << (8 * i);
    }
    std::string bits;
    bits.reserve(bit_count);
    char byte;
    while (in.get(byte) && bits.size() < bit_count)
    {
      for (size_t i = 0; i < 8 && bits.size() < bit_count; i++)
      {
        if (byte & (1 << (7 - i)))
        {
          bits += '1';
        }
        else
        {
          bits += '0';
        }
      }
    }
    if (bits.size() != bit_count)
    {
      throw std::runtime_error("TRUNCATED_FILE");
    }
    return bits;
  }

  void save_code_table(const duhanina::CodeTable& table, str_t filename)
  {
    std::ofstream out(filename);
    if (!out)
    {
      throw std::runtime_error("INVALID_FILE");
    }
    out << table.total_chars << "\n";
    for (auto it = table.char_to_code.begin(); it != table.char_to_code.end(); it++)
    {
      out << static_cast< int >(it->first) << " " << it->second << "\n";
    }
  }

  duhanina::CodeTable load_code_table(str_t filename)
  {
    std::ifstream in(filename);
    if (!in)
    {
      throw std::runtime_error("FILE_NOT_FOUND");
    }
    duhanina::CodeTable table;
    in >> table.total_chars;
    in.ignore();
    std::string line;
    while (std::getline(in, line))
    {
      if (line.empty())
      {
        continue;
      }
      size_t space_pos = line.find(' ');
      if (space_pos == std::string::npos || space_pos == line.length() - 1)
      {
        continue;
      }
      int char_code = std::stoi(line.substr(0, space_pos));
      std::string code = line.substr(space_pos + 1);
      table.char_to_code[static_cast< char >(char_code)] = code;
      table.code_to_char[code] = static_cast< char >(char_code);
    }
    return table;
  }

  void encode_file_impl(str_t input_file, str_t output_file, const duhanina::CodeTable& table, std::ostream& out)
  {
    std::ifstream in(input_file);
    if (!in)
    {
      throw std::runtime_error("FILE_NOT_FOUND");
    }
    std::string text;
    char ch;
    while (in.get(ch))
    {
      text += ch;
    }
    std::string encoded = encode_text(text, table);
    write_bits_to_file(encoded, output_file);
    double original_size = text.size();
    double compressed_size = std::ceil(encoded.size() / 8.0) + sizeof(size_t);
    double ratio = (compressed_size / original_size) * 100;
    out << "File successfully compressed:\n";
    out << "Original size: " << original_size << " bytes\n";
    out << "Compressed size: " << compressed_size << " bytes\n";
    out << "Compression ratio: " << std::fixed << std::setprecision(2) << ratio << "%\n";
  }

  void decode_file_impl(str_t input_file, str_t output_file, const duhanina::CodeTable& table, std::ostream& out)
  {
    std::string encoded = read_bits_from_file(input_file);
    std::string decoded = decode_text(encoded, table);
    std::ofstream out_file(output_file);
    if (!out_file)
    {
      throw std::runtime_error("INVALID_FILE");
    }
    out_file << decoded;
    out << "File successfully decompressed to '" << output_file << "'\n";
  }

  duhanina::List< char > find_missing_chars(str_t text, const duhanina::Tree< char, std::string, std::less< char > >& char_to_code)
  {
    duhanina::List< char > missing;
    for (size_t i = 0; i < text.size(); i++)
    {
      char c = text[i];
      if (char_to_code.find(c) == char_to_code.cend())
      {
        missing.push_back(c);
      }
    }
    return missing;
  }

  void print_missing_chars(const duhanina::List< char >& missing, std::ostream& out)
  {
    if (missing.empty())
    {
      return;
    }
    out << "Missing characters (" << missing.size() << "): ";
    for (auto it = missing.begin(); it != missing.end(); ++it)
    {
      char c = *it;
      if (std::isprint(c))
      {
        out << "'" << c << "' ";
      }
      else
      {
        out << "[0x" << std::hex << static_cast< int >(c) << "] ";
      }
    }
    out << "\n";
  }
}

void duhanina::build_codes(str_t input_file, str_t encoding_id, std::ostream& out)
{
  validate_extension(input_file, TEXT_EXT);
  if (encoding_store.count(encoding_id) > 0)
  {
    throw std::runtime_error("ID_EXISTS");
  }
  std::ifstream in(input_file);
  if (!in)
  {
    throw std::runtime_error("FILE_NOT_FOUND");
  }
  std::string text;
  char ch;
  while (in.get(ch))
  {
    text += ch;
  }
  CodeTable table = build_code_table(text);
  encoding_store[encoding_id] = table;
  out << "Code table successfully built and saved with ID '" << encoding_id << "'\n";
}

void duhanina::show_codes(str_t encoding_id, std::ostream& out)
{
  auto it = encoding_store.find(encoding_id);
  if (it == encoding_store.end())
  {
    throw std::runtime_error("NO_SUCH_ID");
  }
  const CodeTable& table = it->second;
  out << "Code table for " << encoding_id << ":\n";
  out << "Char\tCode\n";
  for (auto it = table.char_to_code.begin(); it != table.char_to_code.end(); it++)
  {
    if (std::isprint(it->first) && !std::isspace(it->first))
    {
      out << "'" << it->first << "'\t" << it->second << "\n";
    }
    else
    {
      out << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast< int >(it->first) << std::dec << "\t" << it->second << "\n";
    }
  }
  out << "Total characters: " << table.total_chars << "\n";
}

void duhanina::save_codes(str_t encoding_id, str_t output_file, std::ostream& out)
{
  validate_extension(output_file, COMPRESSED_EXT);
  auto it = encoding_store.find(encoding_id);
  if (it == encoding_store.end())
  {
    throw std::runtime_error("NO_SUCH_ID");
  }
  save_code_table(it->second, output_file);
  out << "Code table successfully saved to file '" << output_file << "'\n";
}

void duhanina::load_codes(str_t codes_file, str_t encoding_id, std::ostream& out)
{
  validate_extension(codes_file, CODE_TABLE_EXT);
  if (encoding_store.count(encoding_id) > 0)
  {
    throw std::runtime_error("ID_EXISTS");
  }
  CodeTable table = load_code_table(codes_file);
  encoding_store[encoding_id] = table;
  out << "Code table successfully loaded from file '" << codes_file << "' with ID '" << encoding_id << "'\n";
}

void duhanina::clear_codes(str_t encoding_id, std::ostream& out)
{
  if (encoding_store.erase(encoding_id) > 0)
  {
    out << "Code table '" << encoding_id << "' successfully removed\n";
  }
  else
  {
    throw std::runtime_error("NO_SUCH_ID");
  }
}

void duhanina::encode_file_with_codes(str_t input_file, str_t output_file, str_t codes_file, std::ostream& out)
{
  validate_extension(codes_file, CODE_TABLE_EXT);
  validate_extension(input_file, TEXT_EXT);
  validate_extension(output_file, COMPRESSED_EXT);
  CodeTable table = load_code_table(codes_file);
  encode_file_impl(input_file, output_file, table, out);
}

void duhanina::decode_file_with_codes(str_t input_file, str_t output_file, str_t codes_file, std::ostream& out)
{
  validate_extension(codes_file, CODE_TABLE_EXT);
  validate_extension(input_file, TEXT_EXT);
  validate_extension(output_file, COMPRESSED_EXT);
  CodeTable table = load_code_table(codes_file);
  decode_file_impl(input_file, output_file, table, out);
}

void duhanina::encode_file(str_t input_file, str_t output_file, str_t encoding_id, std::ostream& out)
{
  validate_extension(input_file, TEXT_EXT);
  validate_extension(output_file, COMPRESSED_EXT);
  auto it = encoding_store.find(encoding_id);
  if (it == encoding_store.end())
  {
    throw std::runtime_error("NO_SUCH_ID");
  }
  encode_file_impl(input_file, output_file, it->second, out);
}

void duhanina::decode_file(str_t input_file, str_t output_file, str_t encoding_id, std::ostream& out)
{
  validate_extension(input_file, TEXT_EXT);
  validate_extension(output_file, COMPRESSED_EXT);
  auto it = encoding_store.find(encoding_id);
  if (it == encoding_store.end())
  {
    throw std::runtime_error("NO_SUCH_ID");
  }
  decode_file_impl(input_file, output_file, it->second, out);
}

void duhanina::compare(str_t file1, str_t file2, str_t encod_id1, str_t encod_id2, std::ostream& out)
{
  validate_extension(file1, TEXT_EXT);
  validate_extension(file2, TEXT_EXT);
  if (file1 == file2)
  {
    throw std::runtime_error("IDENTICAL_TEXTS");
  }
  auto it1 = encoding_store.find(encod_id1);
  auto it2 = encoding_store.find(encod_id2);
  if (it1 == encoding_store.end() || it2 == encoding_store.end())
  {
    throw std::runtime_error("NO_SUCH_ID");
  }
  std::ifstream in1(file1), in2(file2);
  if (!in1 || !in2)
  {
    throw std::runtime_error("FILE_NOT_FOUND");
  }
  std::string text1;
  char ch1;
  while (in1.get(ch1))
  {
    text1 += ch1;
  }
  std::string text2;
  char ch2;
  while (in2.get(ch2))
  {
    text2 += ch2;
  }
  if (text1 == text2)
  {
    throw std::runtime_error("IDENTICAL_TEXTS");
  }
  std::string encoded1 = encode_text(text1, it1->second);
  std::string encoded2 = encode_text(text2, it2->second);
  double size1_orig = text1.size();
  double size1_comp = std::ceil(encoded1.size() / 8.0) + sizeof(size_t);
  double ratio1 = size1_comp / size1_orig;
  double size2_orig = text2.size();
  double size2_comp = std::ceil(encoded2.size() / 8.0) + sizeof(size_t);
  double ratio2 = size2_comp / size2_orig;
  out << "Compression efficiency comparison:\n";
  out << "----------------------------------------\n";
  out << "File 1: " << file1 << " (encoding: " << encod_id1 << ")\n";
  out << "  Original size: " << size1_orig << " bytes\n";
  out << "  Compressed size: " << size1_comp << " bytes\n";
  out << "  Compression ratio: " << std::fixed << std::setprecision(2) << (ratio1 * 100) << "%\n";
  out << "----------------------------------------\n";
  out << "File 2: " << file2 << " (encoding: " << encod_id2 << ")\n";
  out << "  Original size: " << size2_orig << " bytes\n";
  out << "  Compressed size: " << size2_comp << " bytes\n";
  out << "  Compression ratio: " << std::fixed << std::setprecision(2) << (ratio2 * 100) << "%\n";
  out << "----------------------------------------\n";
  out << "Difference in compression ratio: " << std::fixed << std::setprecision(2) << std::fabs(ratio1 - ratio2) * 100 << "%\n";
}

void duhanina::list_encodings(std::ostream& out)
{
  if (encoding_store.empty())
  {
    out << "No saved encodings available\n";
    return;
  }
  out << "Available encodings:\n";
  for (auto it = encoding_store.begin(); it != encoding_store.end(); it++)
  {
    out << "  " << it->first << " (" << it->second.char_to_code.size() << " symbols, total " << it->second.total_chars << ")\n";
  }
}

void duhanina::print_help(std::ostream& out)
{
  out << "Available commands:\n";
  out << "build_codes <input_file> <encoding_id> - build code table\n";
  out << "show_codes <encoding_id> - show code table\n";
  out << "save_codes <encoding_id> <output_file> - save code table\n";
  out << "load_codes <input_file> <encoding_id> - load code table\n";
  out << "clear_codes <encoding_id> - remove code table\n";
  out << "encode_file_with_codes <input> <output> <codes_file> - compress file\n";
  out << "decode_file_with_codes <input> <output> <codes_file> - decompress file\n";
  out << "encode_file <input> <output> <encoding_id> - compress file\n";
  out << "decode_file <input> <output> <encoding_id> - decompress file\n";
  out << "compare <file1> <file2> <encoding_id1> <encoding_id2> - compare efficiency\n";
  out << "list_encodings - list all encodings\n";
  out << "check_encoding <input_file> <encoding_id> - verify if encoding supports all file characters\n";
  out << "suggest_encodings <input_file> - show which encodings best support the file\n";
  out << "--help - show this help\n";
}

void duhanina::check_encoding(str_t input_file, str_t encoding_id, std::ostream& out)
{
  auto it = encoding_store.find(encoding_id);
  if (it == encoding_store.end())
  {
    throw std::runtime_error("Encoding '" + encoding_id + "' not found");
  }
  std::ifstream in(input_file);
  if (!in)
  {
    throw std::runtime_error("FILE_NOT_FOUND");
  }
  std::string text;
  char ch;
  while (in.get(ch))
  {
    text += ch;
  }
  auto missing = find_missing_chars(text, it->second.char_to_code);
  if (missing.empty())
  {
    out << "Encoding fully supports the file\n";
  }
  else
  {
    out << "Encoding issues in '" << encoding_id << "':\n";
    print_missing_chars(missing, out);
  }
}

void duhanina::suggest_encodings(str_t input_file, std::ostream& out)
{
  std::ifstream in(input_file);
  if (!in)
  {
    throw std::runtime_error("FILE_NOT_FOUND");
  }
  std::string text;
  char ch;
  while (in.get(ch))
  {
    text += ch;
  }
  out << "Encoding compatibility report:\n";
  for (auto encoding_it = encoding_store.begin(); encoding_it != encoding_store.end(); ++encoding_it)
  {
    str_t id = encoding_it->first;
    const CodeTable& table = encoding_it->second;
    duhanina::List< char > missing = find_missing_chars(text, table.char_to_code);
    if (missing.empty())
    {
      out << " - " << id << ": " << "FULL" << " support\n";
    }
    else
    {
      out << " - " << id << ": " << "partial" << " support\n";
    }
  }
}

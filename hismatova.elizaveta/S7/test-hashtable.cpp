#include <boost/test/unit_test.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <cstdio>
#include <sstream>
#include "graph.hpp"
using namespace hismatova;
class TestOutput
{
private:
  std::vector< std::string > lines;
  std::string currentLine;

public:
  void addChar(char c)
  {
    if (c == '\n')
    {
      if (!currentLine.empty())
      {
        lines.push_back(currentLine);
        currentLine.clear();
      }
    }
    else
    {
      currentLine += c;
    }
  }

  std::string getOutput()
  {
    if (!currentLine.empty())
    {
      lines.push_back(currentLine);
      currentLine.clear();
    }

    std::string result;
    for (size_t i = 0; i < lines.size(); ++i)
    {
      result += lines[i];
      if (i < lines.size() - 1)
      {
        result += '\n';
      }
    }
    return result;
  }

  bool contains(const std::string& substr)
  {
    std::string output = getOutput();
    return output.find(substr) != std::string::npos;
  }

  size_t countSubstr(const std::string& substr)
  {
    std::string output = getOutput();
    size_t count = 0;
    size_t pos = 0;
    while ((pos = output.find(substr, pos)) != std::string::npos)
    {
      count++;
      pos += substr.length();
    }
    return count;
  }

  void clear()
  {
    lines.clear();
    currentLine.clear();
  }
};

TestOutput testOutput;

std::streambuf* originalCout = nullptr;

void redirectOutput()
{
  originalCout = std::cout.rdbuf();
  std::cout.rdbuf(new std::stringbuf());
}

void restoreOutput()
{
  if (originalCout)
  {
    std::cout.rdbuf(originalCout);
    originalCout = nullptr;
  }
}

void captureOutput(const std::string& output)
{
  for (char c: output)
  {
    testOutput.addChar(c);
  }
}

BOOST_AUTO_TEST_SUITE(HashtableTests)
BOOST_AUTO_TEST_CASE(test_hashtable_basic)
{
  HashTable< std::string, int > table;

  table["test1"] = 100;
  table["test2"] = 200;

  BOOST_CHECK(table.contains("test1"));
  BOOST_CHECK(table.contains("test2"));
  BOOST_CHECK(!table.contains("test3"));
  BOOST_CHECK_EQUAL(table["test1"], 100);
  BOOST_CHECK_EQUAL(table["test2"], 200);
  BOOST_CHECK_EQUAL(table.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_hashtable_erase)
{
  HashTable< std::string, int > table;

  table["key1"] = 1;
  table["key2"] = 2;
  table["key3"] = 3;

  BOOST_CHECK_EQUAL(table.erase("key2"), 1);
  BOOST_CHECK_EQUAL(table.size(), 2);
  BOOST_CHECK(!table.contains("key2"));
  BOOST_CHECK(table.contains("key1"));
  BOOST_CHECK(table.contains("key3"));
}

BOOST_AUTO_TEST_CASE(test_graphs_command)
{
  GraphProcessor processor;

  std::ofstream file("test_data.txt");
  file << "graph1 2\n";
  file << "a b 10\n";
  file << "b c 20\n";
  file << "graph2 1\n";
  file << "x y 5\n";
  file.close();

  processor.loadGraphsFromFile("test_data.txt");

  testOutput.clear();
  redirectOutput();

  processor.executeCommand("graphs");

  std::string output = dynamic_cast< std::stringbuf* >(std::cout.rdbuf())->str();
  restoreOutput();
  captureOutput(output);

  BOOST_CHECK(testOutput.contains("graph1"));
  BOOST_CHECK(testOutput.contains("graph2"));

  std::remove("test_data.txt");
}

BOOST_AUTO_TEST_CASE(test_vertexes_command)
{
  GraphProcessor processor;

  std::ofstream file("test_data.txt");
  file << "graph1 2\n";
  file << "a b 10\n";
  file << "b c 20\n";
  file.close();

  processor.loadGraphsFromFile("test_data.txt");

  testOutput.clear();
  redirectOutput();

  processor.executeCommand("vertexes graph1");

  std::string output = dynamic_cast< std::stringbuf* >(std::cout.rdbuf())->str();
  restoreOutput();
  captureOutput(output);

  BOOST_CHECK(testOutput.contains("a"));
  BOOST_CHECK(testOutput.contains("b"));
  BOOST_CHECK(testOutput.contains("c"));

  std::remove("test_data.txt");
}

BOOST_AUTO_TEST_CASE(test_outbound_command)
{
  GraphProcessor processor;

  std::ofstream file("test_data.txt");
  file << "graph1 2\n";
  file << "a b 10\n";
  file << "a c 15\n";
  file.close();

  processor.loadGraphsFromFile("test_data.txt");

  testOutput.clear();
  redirectOutput();

  processor.executeCommand("outbound graph1 a");

  std::string output = dynamic_cast< std::stringbuf* >(std::cout.rdbuf())->str();
  restoreOutput();
  captureOutput(output);

  BOOST_CHECK(testOutput.contains("b 10"));
  BOOST_CHECK(testOutput.contains("c 15"));

  std::remove("test_data.txt");
}

BOOST_AUTO_TEST_CASE(test_inbound_command)
{
  GraphProcessor processor;

  std::ofstream file("test_data.txt");
  file << "graph1 3\n";
  file << "a b 10\n";
  file << "c b 15\n";
  file << "d b 20\n";
  file.close();

  processor.loadGraphsFromFile("test_data.txt");

  testOutput.clear();
  redirectOutput();

  processor.executeCommand("inbound graph1 b");

  std::string output = dynamic_cast< std::stringbuf* >(std::cout.rdbuf())->str();
  restoreOutput();
  captureOutput(output);

  BOOST_CHECK(testOutput.contains("a 10"));
  BOOST_CHECK(testOutput.contains("c 15"));
  BOOST_CHECK(testOutput.contains("d 20"));

  std::remove("test_data.txt");
}

BOOST_AUTO_TEST_CASE(test_bind_command)
{
  GraphProcessor processor;

  std::ofstream file("test_data.txt");
  file << "graph1 1\n";
  file << "a b 10\n";
  file.close();

  processor.loadGraphsFromFile("test_data.txt");

  processor.executeCommand("bind graph1 a c 25");

  testOutput.clear();
  redirectOutput();

  processor.executeCommand("outbound graph1 a");

  std::string output = dynamic_cast< std::stringbuf* >(std::cout.rdbuf())->str();
  restoreOutput();
  captureOutput(output);

  BOOST_CHECK(testOutput.contains("b 10"));
  BOOST_CHECK(testOutput.contains("c 25"));

  std::remove("test_data.txt");
}

BOOST_AUTO_TEST_CASE(test_cut_command)
{
  GraphProcessor processor;

  std::ofstream file("test_data.txt");
  file << "graph1 2\n";
  file << "a b 10\n";
  file << "a c 15\n";
  file.close();

  processor.loadGraphsFromFile("test_data.txt");

  processor.executeCommand("cut graph1 a b 10");

  testOutput.clear();
  redirectOutput();

  processor.executeCommand("outbound graph1 a");

  std::string output = dynamic_cast< std::stringbuf* >(std::cout.rdbuf())->str();
  restoreOutput();
  captureOutput(output);

  BOOST_CHECK(!testOutput.contains("b 10"));
  BOOST_CHECK(testOutput.contains("c 15"));

  std::remove("test_data.txt");
}

BOOST_AUTO_TEST_CASE(test_create_command)
{
  GraphProcessor processor;

  processor.executeCommand("create graph1 3 node1 node2 node3");

  testOutput.clear();
  redirectOutput();

  processor.executeCommand("vertexes graph1");

  std::string output = dynamic_cast< std::stringbuf* >(std::cout.rdbuf())->str();
  restoreOutput();
  captureOutput(output);

  BOOST_CHECK(testOutput.contains("node1"));
  BOOST_CHECK(testOutput.contains("node2"));
  BOOST_CHECK(testOutput.contains("node3"));
}

BOOST_AUTO_TEST_CASE(test_merge_command)
{
  GraphProcessor processor;

  std::ofstream file("test_data.txt");
  file << "graph1 2\n";
  file << "a b 10\n";
  file << "c d 20\n";
  file << "graph2 2\n";
  file << "x y 30\n";
  file << "z w 40\n";
  file.close();

  processor.loadGraphsFromFile("test_data.txt");
  processor.executeCommand("merge graph3 graph1 graph2");

  testOutput.clear();
  redirectOutput();

  processor.executeCommand("vertexes graph3");
  processor.executeCommand("outbound graph3 a");
  processor.executeCommand("outbound graph3 x");

  std::string output = dynamic_cast< std::stringbuf* >(std::cout.rdbuf())->str();
  restoreOutput();
  captureOutput(output);

  BOOST_CHECK(testOutput.contains("a"));
  BOOST_CHECK(testOutput.contains("b"));
  BOOST_CHECK(testOutput.contains("x"));
  BOOST_CHECK(testOutput.contains("y"));

  std::remove("test_data.txt");
}

BOOST_AUTO_TEST_CASE(test_extract_command)
{
  GraphProcessor processor;

  std::ofstream file("test_data.txt");
  file << "graph1 3\n";
  file << "a b 10\n";
  file << "b c 20\n";
  file << "c d 30\n";
  file.close();

  processor.loadGraphsFromFile("test_data.txt");
  processor.executeCommand("extract graph2 graph1 2 a b");

  testOutput.clear();
  redirectOutput();

  processor.executeCommand("vertexes graph2");
  processor.executeCommand("outbound graph2 a");

  std::string output = dynamic_cast< std::stringbuf* >(std::cout.rdbuf())->str();
  restoreOutput();
  captureOutput(output);

  BOOST_CHECK(testOutput.contains("a"));
  BOOST_CHECK(testOutput.contains("b"));
  BOOST_CHECK(testOutput.contains("b 10"));
  BOOST_CHECK(!testOutput.contains("c"));

  std::remove("test_data.txt");
}

BOOST_AUTO_TEST_CASE(test_invalid_commands)
{
  GraphProcessor processor;

  testOutput.clear();
  redirectOutput();

  processor.executeCommand("vertexes nonexistent");
  processor.executeCommand("outbound nonexistent vertex");
  processor.executeCommand("invalid_command");
  processor.executeCommand("create graph1");
  processor.executeCommand("bind graph1 a b");

  std::string output = dynamic_cast< std::stringbuf* >(std::cout.rdbuf())->str();
  restoreOutput();
  captureOutput(output);

  BOOST_CHECK_GE(testOutput.countSubstr("<INVALID COMMAND>"), 4);
}

BOOST_AUTO_TEST_CASE(test_outbound_command_complex)
{
  GraphProcessor processor;

  std::ofstream file("test_data.txt");
  file << "graph1 3\n";
  file << "a b 10\n";
  file << "a c 15\n";
  file << "a b 5\n";
  file.close();

  processor.loadGraphsFromFile("test_data.txt");

  testOutput.clear();
  redirectOutput();

  processor.executeCommand("outbound graph1 a");

  std::string output = dynamic_cast< std::stringbuf* >(std::cout.rdbuf())->str();
  restoreOutput();
  captureOutput(output);

  BOOST_CHECK(testOutput.contains("b 5"));
  BOOST_CHECK(testOutput.contains("b 10"));
  BOOST_CHECK(testOutput.contains("c 15"));

  std::remove("test_data.txt");
}

BOOST_AUTO_TEST_CASE(test_inbound_command_complex)
{
  GraphProcessor processor;

  std::ofstream file("test_data.txt");
  file << "graph1 4\n";
  file << "a b 10\n";
  file << "c b 15\n";
  file << "d b 20\n";
  file << "e b 15\n";
  file.close();

  processor.loadGraphsFromFile("test_data.txt");

  testOutput.clear();
  redirectOutput();

  processor.executeCommand("inbound graph1 b");

  std::string output = dynamic_cast< std::stringbuf* >(std::cout.rdbuf())->str();
  restoreOutput();
  captureOutput(output);

  BOOST_CHECK(testOutput.contains("a 10"));
  BOOST_CHECK(testOutput.contains("c 15"));
  BOOST_CHECK(testOutput.contains("d 20"));
  BOOST_CHECK(testOutput.contains("e 15"));

  std::remove("test_data.txt");
}

BOOST_AUTO_TEST_CASE(test_multiple_edges_same_vertices)
{
  GraphProcessor processor;

  std::ofstream file("test_data.txt");
  file << "graph1 4\n";
  file << "a b 10\n";
  file << "a b 20\n";
  file << "a b 5\n";
  file << "b a 15\n";
  file.close();

  processor.loadGraphsFromFile("test_data.txt");

  testOutput.clear();
  redirectOutput();

  processor.executeCommand("outbound graph1 a");
  processor.executeCommand("inbound graph1 a");

  std::string output = dynamic_cast< std::stringbuf* >(std::cout.rdbuf())->str();
  restoreOutput();
  captureOutput(output);

  std::string result = testOutput.getOutput();
  size_t pos1 = result.find("b 5");
  size_t pos2 = result.find("b 10");
  size_t pos3 = result.find("b 20");
  BOOST_CHECK(pos1 != std::string::npos);
  BOOST_CHECK(pos2 != std::string::npos);
  BOOST_CHECK(pos3 != std::string::npos);
  BOOST_CHECK(pos1 < pos2);
  BOOST_CHECK(pos2 < pos3);
  BOOST_CHECK(result.find("b 15") != std::string::npos);

  std::remove("test_data.txt");
}
BOOST_AUTO_TEST_SUITE_END()

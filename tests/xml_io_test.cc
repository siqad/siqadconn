#include "siqadconn.h"
#include <cstdio>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <utility>

static void check(bool ok) { if (!ok) throw std::runtime_error("XML I/O regression"); }
static std::string read(const std::string &path) {
  std::ifstream file(path);
  return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}
static void sentinel(const std::string &path) { std::ofstream(path) << "sentinel"; }
static boost::property_tree::ptree xml(const std::string &path) {
  boost::property_tree::ptree tree;
  boost::property_tree::read_xml(path, tree);
  return tree;
}
int main(int argc, char **argv) {
  check(argc == 2);
  const std::string base = argv[1];
  const std::string input = base + "/problem.xml", output = base + "/results.xml";
  const std::string other = base + "/other.xml";
  std::ofstream(input) << "<siqad><sim_params><test>value</test></sim_params><layers/><design/></siqad>";
  using Pairs = std::vector<std::pair<std::string, std::string>>;
  using Rows = std::vector<std::vector<std::string>>;
  {
    phys::SiQADConnector conn("test", input, output);
    Pairs pairs{{"1&2", "3"}};
    conn.setExport("db_loc", pairs);
    pairs[0].first = "changed";
    Rows rows{{"01", "-0.5", "7", "1", "2"}};
    conn.setExport("db_charge", std::move(rows));
    conn.setExport("misc", Pairs{{"note", "a&b"}});
    Rows potentials{{"1", "2", "3"}};
    conn.setExport("potential", potentials);
    potentials[0][2] = "changed";
    conn.setExport("electrodes", Rows{{"1", "2", "3", "4", "5"}});
    conn.setExport("db_pot", Rows{{"0", "1", "2", "3"}});
    conn.writeResultsXml();
    auto tree = xml(output);
    check(tree.get<std::string>("sim_out.physloc.dbdot.<xmlattr>.x") == "1&2");
    check(tree.get<std::string>("sim_out.elec_dist.dist") == "01");
    check(tree.get<std::string>("sim_out.elec_dist.dist.<xmlattr>.count") == "7");
    check(tree.get<std::string>("sim_out.misc.note") == "a&b");
    check(tree.get<std::string>("sim_out.sim_params.test") == "value");
    check(tree.get<std::string>("sim_out.potential_map.potential_val.<xmlattr>.val") == "3");
    check(tree.get<std::string>("sim_out.electrode.potential") == "5");
    check(tree.get<std::string>("sim_out.dbdots.dbdot.potential") == "3");
    check(tree.get_child("sim_out").size() == 8);
    check(read(output).find("x=\"1&amp;2\"") != std::string::npos);
    sentinel(output);
    conn.setOutputPath(output);
  }
  check(read(output) == "sentinel");
  {
    phys::SiQADConnector conn("test", input, output);
    conn.writeResultsXml();
    sentinel(output);
    conn.writeResultsXml();
    check(read(output) != "sentinel");
    sentinel(output);
    try { conn.setExport("invalid", Pairs{}); check(false); }
    catch (const std::invalid_argument &) {}
  }
  check(read(output) == "sentinel");
  {
    phys::SiQADConnector conn("test", input, output);
    conn.writeResultsXml();
    conn.setExport("misc", Pairs{{"updated", "yes"}});
  }
  check(xml(output).get<std::string>("sim_out.misc.updated") == "yes");
  {
    phys::SiQADConnector conn("test", input, output);
    conn.writeResultsXml();
    conn.setOutputPath(other);
  }
  check(xml(other).get<std::string>("sim_out.eng_info.engine") == "test");
  {
    phys::SiQADConnector conn("test", input, output);
    conn.writeResultsXml();
    phys::AggregateCommand command("add");
    conn.addSQCommand(&command);
  }
  check(!xml(output).get<std::string>("sim_out.sqcommands.sqc").empty());
  {
    phys::SiQADConnector conn("test", input, base + "/missing/results.xml");
    try { conn.writeResultsXml(); check(false); }
    catch (const boost::property_tree::xml_parser_error &) {}
    conn.setOutputPath(other);
    sentinel(other);
  }
  check(read(other) != "sentinel");
  sentinel(output);
  { phys::SiQADConnector conn("test", input, output); }
  check(read(output) != "sentinel");
  std::remove(input.c_str()); std::remove(output.c_str()); std::remove(other.c_str());
}

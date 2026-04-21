#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <interloq/file.hxx>
#include <interloq/interlock.hxx>
#include <iostream>
#include <stdexcept>

#ifndef TEST_DATA_DIR
#error "TEST_DATA_DIR not defined"
#endif

using namespace interloq;

TEST_CASE("Basic LeverFrame Parsing from File", "[file_import]") {
  const std::filesystem::path example_dir{TEST_DATA_DIR};
  const std::filesystem::path file_{example_dir / "basic_leverframe.yml"};
  InterlockDefinition lever_frame{InterlockDefinition::yaml_load(file_)};
  SECTION("Checking General Info") {
    REQUIRE(lever_frame.name == "Basic Example");
    REQUIRE(lever_frame.leverframe.size() == 3);
  }

  const LeverDefinition lever_1_{lever_frame.leverframe[1]},
      lever_2_{lever_frame.leverframe[2]}, lever_3_{lever_frame.leverframe[3]};

  SECTION("Leverframe validation") {
    REQUIRE(lever_1_.id == 1);
    REQUIRE(lever_2_.id == 2);
    REQUIRE(lever_3_.id == 3);

    REQUIRE(lever_1_.name == "Up Main Home");
    REQUIRE(lever_2_.name == "Down Main Home");
    REQUIRE(lever_3_.name == "Up Junction");

    REQUIRE(lever_1_.type == LeverType::StopSignal);
    REQUIRE(lever_2_.type == LeverType::StopSignal);
    REQUIRE(lever_3_.type == LeverType::Points);

    REQUIRE(lever_1_.dependencies.has_value());
    REQUIRE(!lever_2_.dependencies.has_value());
    REQUIRE(lever_3_.dependencies.has_value());

    REQUIRE(lever_1_.dependencies.value().size() == 1);
    REQUIRE(lever_3_.dependencies.value().size() == 1);
  }
  SECTION("Lever Dependency Checking") {
    const std::vector<LeverDependency> lever_1_dependencies_{
        lever_1_.dependencies.value()},
        lever_3_dependencies_{lever_3_.dependencies.value()};

    REQUIRE(lever_1_dependencies_[0].target == 3);
    REQUIRE(!lever_1_dependencies_[0].required_state);
    REQUIRE(lever_3_dependencies_[0].target == 1);
    REQUIRE(lever_3_dependencies_[0].required_state);
  }
}

TEST_CASE("Check allowed movements", "[interlocking_validation]") {
  const std::filesystem::path example_dir{TEST_DATA_DIR};
  const std::filesystem::path file_{example_dir / "basic_leverframe.yml"};
  Interlocking lever_frame{file_};
  REQUIRE(lever_frame.move_lever(1, true));
  REQUIRE(!lever_frame.move_lever(3, true));
}
cmake_minimum_required(VERSION 3.10)

project(PortfolioAnalyzer
  DESCRIPTION "Portfolio Analyzer"
  LANGUAGES CXX)

set(SOURCES
  "Source/AssetClass.cpp"
  "Source/AssetClass.hpp"
  "Source/AssetClassBuilder.cpp"
  "Source/AssetClassBuilder.hpp"
  "Source/CagrHeatMap.cpp"
  "Source/CagrHeatMap.hpp"
  "Source/Drawdowns.cpp"
  "Source/Drawdowns.hpp"
  "Source/Main.cpp"
  "Source/Percentile.cpp"
  "Source/Percentile.hpp"
  "Source/Portfolio.cpp"
  "Source/Portfolio.hpp"
  "Source/PortfolioBuilder.cpp"
  "Source/PortfolioBuilder.hpp"
  "Source/RollingReturns.cpp"
  "Source/RollingReturns.hpp"
  "Source/Types.hpp")

set(PRIVATE_INCLUDE_DIRECTORIES
  "External/rapidjson/include"
  "Source")

add_executable(PortfolioAnalyzer ${SOURCES})

target_include_directories(
  PortfolioAnalyzer
  PRIVATE
  ${PRIVATE_INCLUDE_DIRECTORIES})

set_property(
  TARGET PortfolioAnalyzer
  PROPERTY CXX_STANDARD 17)

source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR}
  FILES ${SOURCES})

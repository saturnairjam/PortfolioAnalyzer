cmake_minimum_required(VERSION 3.10)

project(PortfolioAnalyzer
  DESCRIPTION "Portfolio Analyzer"
  LANGUAGES CXX)

set(SOURCES
  "Source/Main.cpp")

set(PRIVATE_INCLUDE_DIRECTORIES "Source")

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

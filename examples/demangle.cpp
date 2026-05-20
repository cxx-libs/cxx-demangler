#include <cxx/demangler/demangler.hpp>
#include <iostream>
#include <string_view>
#include <vector>

int main()
{
  std::vector<std::string_view> mangleds{
    "_Z14lambda_factoryIiEDav",
    "_Z17gigantic_symbol_0IiEvN11StressType0IT_E4typeE",
    "_Z17gigantic_symbol_1IiEvN11StressType1IT_E4typeE",
    "_Z17gigantic_symbol_2IiEvN11StressType2IT_E4typeE",
    "_Z17gigantic_symbol_3IiEvN11StressType3IT_E4typeE",
    "_Z18gigantic_symbol_19IiEvN12StressType19IT_E4typeE",
    "_Z18gigantic_symbol_20IiEvN12StressType20IT_E4typeE",
    "_Z18gigantic_symbol_21IiEvN12StressType21IT_E4typeE",
    "_Z18gigantic_symbol_22IiEvN12StressType22IT_E4typeE",
  };
  cxx::demangler::Demangler my_demangler;
  std::cout << cxx::demangler::Demangler::call( "_ZNSt6vectorINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESaIS5_EEC1Ev" ) << std::endl;
  std::cout << my_demangler( "_ZNSt6vectorINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESaIS5_EEC1Ev" ) << std::endl;
  for( const auto& item: mangleds ) { std::cout << my_demangler.call( item ) << std::endl; }

  std::cout << "OS backend" << std::endl;
  for( const auto& item: mangleds ) { std::cout << my_demangler.call( item, cxx::demangler::Demangler::Backend::os ) << std::endl; }
}

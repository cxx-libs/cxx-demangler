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
    "?func@@YAXH@Z",
    "??_C@_04CEJDCDCH@test?$AA@ `string'",
    "??id$initializer$@?$numpunct@D@std@@2P6AXXZA@@3P6AXXZA ;$initializer$",
    "??__K_w@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PB_SI@Z operator "
    " _w",
    "?fxunc@A@@QGAEHXZ A::fxunc ; ref_specifier",
    "?fxunc@A@@QGBEHXZ A::fxunc",
    "?fxunc@A@@QGCEHXZ A::fxunc",
    "?fxunc@A@@QGDEHXZ A::fxunc",
    "?fxunc@A@@QHAEHXZ A::fxunc",
    "?fxunc@A@@QHBEHXZ A::fxunc",
    "?fxunc@A@@QHCEHXZ A::fxunc",
    "?fxunc@A@@QHDEHXZ A::fxunc",
    "??$_Move_backward_opt@PAU?$digraph@_W@re_detail@boost@@PAU123@Urandom_access_iterator_tag@std@@U_Undefined_move_tag@5@@std@@YAPAU?$digraph@_W@re_detail@boost@@PAU123@00Urandom_access_iterator_tag@0@U_Undefined_move_tag@0@U_Range_checked_iterator_tag@0@@Z ; backref, string",
  };
  cxx::demangler::Demangler my_demangler;
  //std::cout << cxx::demangler::Demangler::call( "_ZNSt6vectorINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESaIS5_EEC1Ev" ) << std::endl;
  //std::cout << my_demangler( "_ZNSt6vectorINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESaIS5_EEC1Ev" ) << std::endl;
  std::cout << "Clang backend" << std::endl;
  for( const auto& item: mangleds ) { std::cout << item << "\n(unmangled)" << my_demangler.call( item ) << "\n\n"; }

  std::cout << "OS backend" << std::endl;
  for( const auto& item: mangleds ) { std::cout << item << "\n(unmangled)" << my_demangler.call( item, cxx::demangler::Demangler::Backend::os ) << "\n\n"; }
}

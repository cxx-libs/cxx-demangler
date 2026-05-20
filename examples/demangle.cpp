#include <cxx/demangler/demangler.hpp>
#include <iostream>
#include <string_view>
#include <vector>

int main()
{
  std::vector<std::string_view> mangleds{
    "_Z14lambda_factoryIiEDav",
    "_Z15stress_functionIiEvN7MonsterIT_E4typeESt5tupleIJS4_IJS4_IJS4_IJNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESt6vectorISA_SaISA_EESt3mapISA_SA_St4lessISA_ESaISt4pairIKSA_SA_EEEEESB_ISM_SaISM_EESE_ISA_SM_SG_SaISH_ISI_SM_EEEEESB_ISS_SaISS_EESE_ISA_SS_SG_SaISH_ISI_SS_EEEEESB_ISY_SaISY_EESE_ISA_SY_SG_SaISH_ISI_SY_EEEEESt10shared_ptrIS4_IJSB_IS1_SaIS1_EESE_ISA_S1_SG_SaISH_ISI_S1_EEESt8optionalIS1_ESY_EEEDTcl14lambda_factoryIS1_EEE",
    "_Z17gigantic_symbol_0IiEvN11StressType0IT_E4typeE",
    "_Z17gigantic_symbol_1IiEvN11StressType1IT_E4typeE",
    "_Z17gigantic_symbol_2IiEvN11StressType2IT_E4typeE",
    "_Z17gigantic_symbol_3IiEvN11StressType3IT_E4typeE",
    "_Z18gigantic_symbol_19IiEvN12StressType19IT_E4typeE",
    "_Z18gigantic_symbol_20IiEvN12StressType20IT_E4typeE",
    "_Z18gigantic_symbol_21IiEvN12StressType21IT_E4typeE",
    "_Z18gigantic_symbol_22IiEvN12StressType22IT_E4typeE",
    "_ZNSt6vectorISt5tupleIJS0_IJNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES_IS6_SaIS6_EESt3mapIS6_S6_St4lessIS6_ESaISt4pairIKS6_S6_EEEEES_ISH_SaISH_EES9_IS6_SH_SB_SaISC_ISD_SH_EEEEESaISN_EEC1Ev",
    "_ZNSt6vectorISt5tupleIJS0_IJNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES_IS6_SaIS6_EESt3mapIS6_S6_St4lessIS6_ESaISt4pairIKS6_S6_EEEEES_ISH_SaISH_EES9_IS6_SH_SB_SaISC_ISD_SH_EEEEESaISN_EEC2Ev",
  };
  cxx::demangler::Demangler my_demangler;
  std::cout << cxx::demangler::Demangler::call( "_ZNSt6vectorINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESaIS5_EEC1Ev" ) << std::endl;
  std::cout << my_demangler( "_ZNSt6vectorINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESaIS5_EEC1Ev" ) << std::endl;
  for( const auto& item: mangleds ) { std::cout << my_demangler.call( item ) << std::endl; }
}

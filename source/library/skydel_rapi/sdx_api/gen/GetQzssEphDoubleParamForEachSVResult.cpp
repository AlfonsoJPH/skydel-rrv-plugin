#include "command_factory.h"
#include "command_result_factory.h"
#include "parse_json.hpp"

///
/// Definition of GetQzssEphDoubleParamForEachSVResult
///
#include "gen/GetQzssEphDoubleParamForEachSVResult.h"

namespace Sdx
{
  namespace Cmd
  {
    const char* const GetQzssEphDoubleParamForEachSVResult::CmdName = "GetQzssEphDoubleParamForEachSVResult";
    const char* const GetQzssEphDoubleParamForEachSVResult::Documentation = "Result of GetQzssEphDoubleParamForEachSV";

    REGISTER_COMMAND_RESULT_FACTORY(GetQzssEphDoubleParamForEachSVResult);


    GetQzssEphDoubleParamForEachSVResult::GetQzssEphDoubleParamForEachSVResult()
      : CommandResult(CmdName)
    {}

    GetQzssEphDoubleParamForEachSVResult::GetQzssEphDoubleParamForEachSVResult(CommandBasePtr relatedCommand, const std::string& paramName, const std::vector<double>& val)
      : CommandResult(CmdName, relatedCommand)
    {

      setParamName(paramName);
      setVal(val);
    }


    GetQzssEphDoubleParamForEachSVResultPtr GetQzssEphDoubleParamForEachSVResult::create(CommandBasePtr relatedCommand, const std::string& paramName, const std::vector<double>& val)
    {
      return GetQzssEphDoubleParamForEachSVResultPtr(new GetQzssEphDoubleParamForEachSVResult(relatedCommand, paramName, val));
    }

    GetQzssEphDoubleParamForEachSVResultPtr GetQzssEphDoubleParamForEachSVResult::dynamicCast(CommandBasePtr ptr)
    {
      return std::dynamic_pointer_cast<GetQzssEphDoubleParamForEachSVResult>(ptr);
    }

    bool GetQzssEphDoubleParamForEachSVResult::isValid() const
    {
      
        return m_values.IsObject()
          && parse_json<std::string>::is_valid(m_values["ParamName"])
          && parse_json<std::vector<double>>::is_valid(m_values["Val"])
        ;

    }

    std::string GetQzssEphDoubleParamForEachSVResult::documentation() const { return Documentation; }


    std::string GetQzssEphDoubleParamForEachSVResult::paramName() const
    {
      return parse_json<std::string>::parse(m_values["ParamName"]);
    }

    void GetQzssEphDoubleParamForEachSVResult::setParamName(const std::string& paramName)
    {
      m_values.AddMember("ParamName", parse_json<std::string>::format(paramName, m_values.GetAllocator()), m_values.GetAllocator());
    }



    std::vector<double> GetQzssEphDoubleParamForEachSVResult::val() const
    {
      return parse_json<std::vector<double>>::parse(m_values["Val"]);
    }

    void GetQzssEphDoubleParamForEachSVResult::setVal(const std::vector<double>& val)
    {
      m_values.AddMember("Val", parse_json<std::vector<double>>::format(val, m_values.GetAllocator()), m_values.GetAllocator());
    }


  }
}

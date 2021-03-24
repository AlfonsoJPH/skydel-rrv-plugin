#include "command_factory.h"
#include "command_result_factory.h"
#include "parse_json.hpp"

///
/// Definition of SetGpsConfigurationForEachSV
///
#include "gen/SetGpsConfigurationForEachSV.h"

namespace Sdx
{
  namespace Cmd
  {
    const char* const SetGpsConfigurationForEachSV::CmdName = "SetGpsConfigurationForEachSV";
    const char* const SetGpsConfigurationForEachSV::Documentation = "Please note the command SetGpsSVConfigurationForAllSat is deprecated since 21.3. You may use SetGpsConfigurationForEachSV.\n\nSet GPS SV configuration flag for each SVs";

    REGISTER_COMMAND_FACTORY(SetGpsConfigurationForEachSV);


    SetGpsConfigurationForEachSV::SetGpsConfigurationForEachSV()
      : CommandBase(CmdName)
    {}

    SetGpsConfigurationForEachSV::SetGpsConfigurationForEachSV(const std::vector<int>& svConfigs)
      : CommandBase(CmdName)
    {

      setSvConfigs(svConfigs);
    }


    SetGpsConfigurationForEachSVPtr SetGpsConfigurationForEachSV::create(const std::vector<int>& svConfigs)
    {
      return SetGpsConfigurationForEachSVPtr(new SetGpsConfigurationForEachSV(svConfigs));
    }

    SetGpsConfigurationForEachSVPtr SetGpsConfigurationForEachSV::dynamicCast(CommandBasePtr ptr)
    {
      return std::dynamic_pointer_cast<SetGpsConfigurationForEachSV>(ptr);
    }

    bool SetGpsConfigurationForEachSV::isValid() const
    {
      
        return m_values.IsObject()
          && parse_json<std::vector<int>>::is_valid(m_values["SvConfigs"])
        ;

    }

    std::string SetGpsConfigurationForEachSV::documentation() const { return Documentation; }


    int SetGpsConfigurationForEachSV::executePermission() const
    {
      return EXECUTE_IF_IDLE | EXECUTE_IF_SIMULATING;
    }


    std::vector<int> SetGpsConfigurationForEachSV::svConfigs() const
    {
      return parse_json<std::vector<int>>::parse(m_values["SvConfigs"]);
    }

    void SetGpsConfigurationForEachSV::setSvConfigs(const std::vector<int>& svConfigs)
    {
      m_values.AddMember("SvConfigs", parse_json<std::vector<int>>::format(svConfigs, m_values.GetAllocator()), m_values.GetAllocator());
    }


  }
}

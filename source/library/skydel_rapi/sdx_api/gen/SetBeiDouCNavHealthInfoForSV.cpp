#include "command_factory.h"
#include "command_result_factory.h"
#include "parse_json.hpp"

///
/// Definition of SetBeiDouCNavHealthInfoForSV
///
#include "gen/SetBeiDouCNavHealthInfoForSV.h"

namespace Sdx
{
  namespace Cmd
  {
    const char* const SetBeiDouCNavHealthInfoForSV::CmdName = "SetBeiDouCNavHealthInfoForSV";
    const char* const SetBeiDouCNavHealthInfoForSV::Documentation = "Set BeiDou CNav satellite health info";

    REGISTER_COMMAND_FACTORY(SetBeiDouCNavHealthInfoForSV);


    SetBeiDouCNavHealthInfoForSV::SetBeiDouCNavHealthInfoForSV()
      : CommandBase(CmdName)
    {}

    SetBeiDouCNavHealthInfoForSV::SetBeiDouCNavHealthInfoForSV(int svId, int health)
      : CommandBase(CmdName)
    {

      setSvId(svId);
      setHealth(health);
    }


    SetBeiDouCNavHealthInfoForSVPtr SetBeiDouCNavHealthInfoForSV::create(int svId, int health)
    {
      return SetBeiDouCNavHealthInfoForSVPtr(new SetBeiDouCNavHealthInfoForSV(svId, health));
    }

    SetBeiDouCNavHealthInfoForSVPtr SetBeiDouCNavHealthInfoForSV::dynamicCast(CommandBasePtr ptr)
    {
      return std::dynamic_pointer_cast<SetBeiDouCNavHealthInfoForSV>(ptr);
    }

    bool SetBeiDouCNavHealthInfoForSV::isValid() const
    {
      
        return m_values.IsObject()
          && parse_json<int>::is_valid(m_values["SvId"])
          && parse_json<int>::is_valid(m_values["Health"])
        ;

    }

    std::string SetBeiDouCNavHealthInfoForSV::documentation() const { return Documentation; }


    int SetBeiDouCNavHealthInfoForSV::executePermission() const
    {
      return EXECUTE_IF_SIMULATING | EXECUTE_IF_IDLE;
    }


    int SetBeiDouCNavHealthInfoForSV::svId() const
    {
      return parse_json<int>::parse(m_values["SvId"]);
    }

    void SetBeiDouCNavHealthInfoForSV::setSvId(int svId)
    {
      m_values.AddMember("SvId", parse_json<int>::format(svId, m_values.GetAllocator()), m_values.GetAllocator());
    }



    int SetBeiDouCNavHealthInfoForSV::health() const
    {
      return parse_json<int>::parse(m_values["Health"]);
    }

    void SetBeiDouCNavHealthInfoForSV::setHealth(int health)
    {
      m_values.AddMember("Health", parse_json<int>::format(health, m_values.GetAllocator()), m_values.GetAllocator());
    }


  }
}

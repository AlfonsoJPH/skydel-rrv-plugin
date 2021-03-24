#pragma once

#include <memory>
#include "command_base.h"
#include <string>

namespace Sdx
{
  namespace Cmd
  {
    ///
    /// Get the antenna model used by the SV.
    ///
    /// Name   Type   Description
    /// ------ ------ -----------------------------------------------------------------
    /// System string "GPS", "GLONASS", "Galileo", "BeiDou", "SBAS", "QZSS" or "NavIC".
    /// SvId   int    The satellite's SV ID.
    ///

    class GetSVAntennaModelForSV;
    typedef std::shared_ptr<GetSVAntennaModelForSV> GetSVAntennaModelForSVPtr;
    
    
    class GetSVAntennaModelForSV : public CommandBase
    {
    public:
      static const char* const CmdName;
      static const char* const Documentation;


      GetSVAntennaModelForSV();

      GetSVAntennaModelForSV(const std::string& system, int svId);
  
      static GetSVAntennaModelForSVPtr create(const std::string& system, int svId);
      static GetSVAntennaModelForSVPtr dynamicCast(CommandBasePtr ptr);
      virtual bool isValid() const override;
      virtual std::string documentation() const override;

      virtual int executePermission() const override;


      // **** system ****
      std::string system() const;
      void setSystem(const std::string& system);


      // **** svId ****
      int svId() const;
      void setSvId(int svId);
    };
  }
}


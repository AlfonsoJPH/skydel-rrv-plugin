#pragma once

#include <memory>
#include "command_base.h"


namespace Sdx
{
  namespace Cmd
  {
    ///
    /// Please note the command GlonassSatelliteAlmanacUnhealthyFlag is deprecated since 21.3. You may use GlonassAlmanacUnhealthyFlagForSV.
    /// 
    /// Get GLONASS satellite Almanac Unhealthy Flag Cn
    ///
    /// Name Type Description
    /// ---- ---- --------------------------
    /// Prn  int  Satellite PRN number 1..24
    ///

    class GetGlonassSatelliteAlmanacUnhealthyFlag;
    typedef std::shared_ptr<GetGlonassSatelliteAlmanacUnhealthyFlag> GetGlonassSatelliteAlmanacUnhealthyFlagPtr;
    
    
    class GetGlonassSatelliteAlmanacUnhealthyFlag : public CommandBase
    {
    public:
      static const char* const CmdName;
      static const char* const Documentation;


      GetGlonassSatelliteAlmanacUnhealthyFlag();

      GetGlonassSatelliteAlmanacUnhealthyFlag(int prn);
  
      static GetGlonassSatelliteAlmanacUnhealthyFlagPtr create(int prn);
      static GetGlonassSatelliteAlmanacUnhealthyFlagPtr dynamicCast(CommandBasePtr ptr);
      virtual bool isValid() const override;
      virtual std::string documentation() const override;

      virtual int executePermission() const override;


      // **** prn ****
      int prn() const;
      void setPrn(int prn);
    };
  }
}


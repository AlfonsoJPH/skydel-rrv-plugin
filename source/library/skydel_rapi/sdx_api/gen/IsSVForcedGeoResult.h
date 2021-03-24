#pragma once

#include <memory>
#include "command_result.h"
#include <string>

namespace Sdx
{
  namespace Cmd
  {
    ///
    /// Result of IsSVForcedGeo
    ///
    /// Name      Type   Description
    /// --------- ------ ---------------------------------------------
    /// System    string "GPS", "Galileo", "BeiDou", "QZSS" or "NavIC"
    /// SvId      int    The satellite SV ID
    /// IsGeo     bool   True for geostationary satellite
    /// Longitude double The longitude to use, in degree
    ///

    class IsSVForcedGeoResult;
    typedef std::shared_ptr<IsSVForcedGeoResult> IsSVForcedGeoResultPtr;
    
    
    class IsSVForcedGeoResult : public CommandResult
    {
    public:
      static const char* const CmdName;
      static const char* const Documentation;


      IsSVForcedGeoResult();

      IsSVForcedGeoResult(CommandBasePtr relatedCommand, const std::string& system, int svId, bool isGeo, double longitude);
  
      static IsSVForcedGeoResultPtr create(CommandBasePtr relatedCommand, const std::string& system, int svId, bool isGeo, double longitude);
      static IsSVForcedGeoResultPtr dynamicCast(CommandBasePtr ptr);
      virtual bool isValid() const override;
      virtual std::string documentation() const override;


      // **** system ****
      std::string system() const;
      void setSystem(const std::string& system);


      // **** svId ****
      int svId() const;
      void setSvId(int svId);


      // **** isGeo ****
      bool isGeo() const;
      void setIsGeo(bool isGeo);


      // **** longitude ****
      double longitude() const;
      void setLongitude(double longitude);
    };
  }
}


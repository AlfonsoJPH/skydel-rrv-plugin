#pragma once

#include <memory>
#include "command_result.h"
#include <string>

namespace Sdx
{
  namespace Cmd
  {
    ///
    /// Result of GetBeiDouD2NavigationMessage
    ///
    /// Name         Type   Description
    /// ------------ ------ -----------------------------------------------------------
    /// Prn          int    Satellite PRN number 1..5
    /// StartTime    int    Elapsed time in seconds since start of simulation
    /// StopTime     int    Elapsed time in seconds since start of simulation
    /// SubFrame     int    Subframe 1..5 (use 0 to apply modification to all subframes
    /// Page         int    Page 1..120 (use 0 to apply modification to all pages)
    /// Word         int    Word 1..10 (use 0 to apply modification to all words)
    /// UpdateParity bool   Recalculate parity after making modification
    /// Modification string Modification string must be 30 bytes long
    /// Id           string Unique identifier of the event
    ///

    class GetBeiDouD2NavigationMessageResult;
    typedef std::shared_ptr<GetBeiDouD2NavigationMessageResult> GetBeiDouD2NavigationMessageResultPtr;
    
    
    class GetBeiDouD2NavigationMessageResult : public CommandResult
    {
    public:
      static const char* const CmdName;
      static const char* const Documentation;


      GetBeiDouD2NavigationMessageResult();

      GetBeiDouD2NavigationMessageResult(CommandBasePtr relatedCommand, int prn, int startTime, int stopTime, int subFrame, int page, int word, bool updateParity, const std::string& modification, const std::string& id);
  
      static GetBeiDouD2NavigationMessageResultPtr create(CommandBasePtr relatedCommand, int prn, int startTime, int stopTime, int subFrame, int page, int word, bool updateParity, const std::string& modification, const std::string& id);
      static GetBeiDouD2NavigationMessageResultPtr dynamicCast(CommandBasePtr ptr);
      virtual bool isValid() const override;
      virtual std::string documentation() const override;


      // **** prn ****
      int prn() const;
      void setPrn(int prn);


      // **** startTime ****
      int startTime() const;
      void setStartTime(int startTime);


      // **** stopTime ****
      int stopTime() const;
      void setStopTime(int stopTime);


      // **** subFrame ****
      int subFrame() const;
      void setSubFrame(int subFrame);


      // **** page ****
      int page() const;
      void setPage(int page);


      // **** word ****
      int word() const;
      void setWord(int word);


      // **** updateParity ****
      bool updateParity() const;
      void setUpdateParity(bool updateParity);


      // **** modification ****
      std::string modification() const;
      void setModification(const std::string& modification);


      // **** id ****
      std::string id() const;
      void setId(const std::string& id);
    };
  }
}


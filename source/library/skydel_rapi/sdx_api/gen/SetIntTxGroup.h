#pragma once

#include <memory>
#include "command_base.h"
#include <string>

namespace Sdx
{
  namespace Cmd
  {
    ///
    /// Set the interference transmitter group (which links to a specific RF output target).
    ///
    /// Name  Type   Description
    /// ----- ------ ---------------------------------
    /// Group int    Interference group number [1..16]
    /// Id    string Transmitter unique identifier.
    ///

    class SetIntTxGroup;
    typedef std::shared_ptr<SetIntTxGroup> SetIntTxGroupPtr;
    
    
    class SetIntTxGroup : public CommandBase
    {
    public:
      static const char* const CmdName;
      static const char* const Documentation;


      SetIntTxGroup();

      SetIntTxGroup(int group, const std::string& id);
  
      static SetIntTxGroupPtr create(int group, const std::string& id);
      static SetIntTxGroupPtr dynamicCast(CommandBasePtr ptr);
      virtual bool isValid() const override;
      virtual std::string documentation() const override;

      virtual int executePermission() const override;


      // **** group ****
      int group() const;
      void setGroup(int group);


      // **** id ****
      std::string id() const;
      void setId(const std::string& id);
    };
  }
}


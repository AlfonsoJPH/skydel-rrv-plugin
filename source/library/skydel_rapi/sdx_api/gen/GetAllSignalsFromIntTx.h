#pragma once

#include <memory>
#include "command_base.h"
#include <string>

namespace Sdx
{
  namespace Cmd
  {
    ///
    /// Get all signal ID for this interference transmitters.
    ///
    /// Name Type   Description
    /// ---- ------ ------------------------------
    /// Id   string Transmitter unique identifier.
    ///

    class GetAllSignalsFromIntTx;
    typedef std::shared_ptr<GetAllSignalsFromIntTx> GetAllSignalsFromIntTxPtr;
    
    
    class GetAllSignalsFromIntTx : public CommandBase
    {
    public:
      static const char* const CmdName;
      static const char* const Documentation;


      GetAllSignalsFromIntTx();

      GetAllSignalsFromIntTx(const std::string& id);
  
      static GetAllSignalsFromIntTxPtr create(const std::string& id);
      static GetAllSignalsFromIntTxPtr dynamicCast(CommandBasePtr ptr);
      virtual bool isValid() const override;
      virtual std::string documentation() const override;

      virtual int executePermission() const override;


      // **** id ****
      std::string id() const;
      void setId(const std::string& id);
    };
  }
}


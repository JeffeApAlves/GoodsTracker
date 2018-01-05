//
// Generated file, do not edit! Created by nedtool 5.2 from veins/modules/messages/BasicSafetyMessage.msg.
//

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef __BASICSAFETYMESSAGE_M_H
#define __BASICSAFETYMESSAGE_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0502
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
#include "veins/base/utils/Coord.h"
#include "veins/modules/messages/WaveShortMessage_m.h"
// }}

/**
 * Class generated from <tt>veins/modules/messages/BasicSafetyMessage.msg:29</tt> by nedtool.
 * <pre>
 * packet BasicSafetyMessage extends WaveShortMessage
 * {
 *     Coord senderPos;
 *     Coord senderSpeed;
 * }
 * </pre>
 */
class BasicSafetyMessage : public ::WaveShortMessage
{
  protected:
    Coord senderPos;
    Coord senderSpeed;

  private:
    void copy(const BasicSafetyMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const BasicSafetyMessage&);

  public:
    BasicSafetyMessage(const char *name=nullptr, short kind=0);
    BasicSafetyMessage(const BasicSafetyMessage& other);
    virtual ~BasicSafetyMessage();
    BasicSafetyMessage& operator=(const BasicSafetyMessage& other);
    virtual BasicSafetyMessage *dup() const override {return new BasicSafetyMessage(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual Coord& getSenderPos();
    virtual const Coord& getSenderPos() const {return const_cast<BasicSafetyMessage*>(this)->getSenderPos();}
    virtual void setSenderPos(const Coord& senderPos);
    virtual Coord& getSenderSpeed();
    virtual const Coord& getSenderSpeed() const {return const_cast<BasicSafetyMessage*>(this)->getSenderSpeed();}
    virtual void setSenderSpeed(const Coord& senderSpeed);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const BasicSafetyMessage& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, BasicSafetyMessage& obj) {obj.parsimUnpack(b);}


#endif // ifndef __BASICSAFETYMESSAGE_M_H

//
// Generated file, do not edit! Created by nedtool 5.4 from EpidemicMessage.msg.
//

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef __EPIDEMICMESSAGE_M_H
#define __EPIDEMICMESSAGE_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0504
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>EpidemicMessage.msg:12</tt> by nedtool.
 * <pre>
 * message epidemicMessage
 * {
 *     int slotTimeCount = 0;
 *     int hopCount = 0;
 * 
 * }
 * </pre>
 */
class epidemicMessage : public ::omnetpp::cMessage
{
  protected:
    int slotTimeCount;
    int hopCount;

  private:
    void copy(const epidemicMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const epidemicMessage&);

  public:
    epidemicMessage(const char *name=nullptr, short kind=0);
    epidemicMessage(const epidemicMessage& other);
    virtual ~epidemicMessage();
    epidemicMessage& operator=(const epidemicMessage& other);
    virtual epidemicMessage *dup() const override {return new epidemicMessage(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getSlotTimeCount() const;
    virtual void setSlotTimeCount(int slotTimeCount);
    virtual int getHopCount() const;
    virtual void setHopCount(int hopCount);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const epidemicMessage& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, epidemicMessage& obj) {obj.parsimUnpack(b);}


#endif // ifndef __EPIDEMICMESSAGE_M_H


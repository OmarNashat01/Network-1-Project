//
// Generated file, do not edit! Created by nedtool 5.6 from Message.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "Message_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(Message)

Message::Message(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->nodeInd = 0;
    this->startTime = 0;
    this->header = 0;
    this->trailer = 0;
    this->frame_type = 0;
    this->ack_nr = 0;
    this->delay = 0;
    this->mod = false;
    this->lost = false;
    this->duplicated = false;
}

Message::Message(const Message& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

Message::~Message()
{
}

Message& Message::operator=(const Message& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void Message::copy(const Message& other)
{
    this->nodeInd = other.nodeInd;
    this->startTime = other.startTime;
    this->header = other.header;
    this->payload = other.payload;
    this->trailer = other.trailer;
    this->frame_type = other.frame_type;
    this->ack_nr = other.ack_nr;
    this->delay = other.delay;
    this->mod = other.mod;
    this->lost = other.lost;
    this->duplicated = other.duplicated;
    this->error = other.error;
}

void Message::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->nodeInd);
    doParsimPacking(b,this->startTime);
    doParsimPacking(b,this->header);
    doParsimPacking(b,this->payload);
    doParsimPacking(b,this->trailer);
    doParsimPacking(b,this->frame_type);
    doParsimPacking(b,this->ack_nr);
    doParsimPacking(b,this->delay);
    doParsimPacking(b,this->mod);
    doParsimPacking(b,this->lost);
    doParsimPacking(b,this->duplicated);
    doParsimPacking(b,this->error);
}

void Message::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->nodeInd);
    doParsimUnpacking(b,this->startTime);
    doParsimUnpacking(b,this->header);
    doParsimUnpacking(b,this->payload);
    doParsimUnpacking(b,this->trailer);
    doParsimUnpacking(b,this->frame_type);
    doParsimUnpacking(b,this->ack_nr);
    doParsimUnpacking(b,this->delay);
    doParsimUnpacking(b,this->mod);
    doParsimUnpacking(b,this->lost);
    doParsimUnpacking(b,this->duplicated);
    doParsimUnpacking(b,this->error);
}

int Message::getNodeInd() const
{
    return this->nodeInd;
}

void Message::setNodeInd(int nodeInd)
{
    this->nodeInd = nodeInd;
}

int Message::getStartTime() const
{
    return this->startTime;
}

void Message::setStartTime(int startTime)
{
    this->startTime = startTime;
}

int Message::getHeader() const
{
    return this->header;
}

void Message::setHeader(int header)
{
    this->header = header;
}

const char * Message::getPayload() const
{
    return this->payload.c_str();
}

void Message::setPayload(const char * payload)
{
    this->payload = payload;
}

char Message::getTrailer() const
{
    return this->trailer;
}

void Message::setTrailer(char trailer)
{
    this->trailer = trailer;
}

int Message::getFrame_type() const
{
    return this->frame_type;
}

void Message::setFrame_type(int frame_type)
{
    this->frame_type = frame_type;
}

int Message::getAck_nr() const
{
    return this->ack_nr;
}

void Message::setAck_nr(int ack_nr)
{
    this->ack_nr = ack_nr;
}

double Message::getDelay() const
{
    return this->delay;
}

void Message::setDelay(double delay)
{
    this->delay = delay;
}

bool Message::getMod() const
{
    return this->mod;
}

void Message::setMod(bool mod)
{
    this->mod = mod;
}

bool Message::getLost() const
{
    return this->lost;
}

void Message::setLost(bool lost)
{
    this->lost = lost;
}

bool Message::getDuplicated() const
{
    return this->duplicated;
}

void Message::setDuplicated(bool duplicated)
{
    this->duplicated = duplicated;
}

const char * Message::getError() const
{
    return this->error.c_str();
}

void Message::setError(const char * error)
{
    this->error = error;
}

class MessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    MessageDescriptor();
    virtual ~MessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(MessageDescriptor)

MessageDescriptor::MessageDescriptor() : omnetpp::cClassDescriptor("Message", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

MessageDescriptor::~MessageDescriptor()
{
    delete[] propertynames;
}

bool MessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<Message *>(obj)!=nullptr;
}

const char **MessageDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *MessageDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int MessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 12+basedesc->getFieldCount() : 12;
}

unsigned int MessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<12) ? fieldTypeFlags[field] : 0;
}

const char *MessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "nodeInd",
        "startTime",
        "header",
        "payload",
        "trailer",
        "frame_type",
        "ack_nr",
        "delay",
        "mod",
        "lost",
        "duplicated",
        "error",
    };
    return (field>=0 && field<12) ? fieldNames[field] : nullptr;
}

int MessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='n' && strcmp(fieldName, "nodeInd")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "startTime")==0) return base+1;
    if (fieldName[0]=='h' && strcmp(fieldName, "header")==0) return base+2;
    if (fieldName[0]=='p' && strcmp(fieldName, "payload")==0) return base+3;
    if (fieldName[0]=='t' && strcmp(fieldName, "trailer")==0) return base+4;
    if (fieldName[0]=='f' && strcmp(fieldName, "frame_type")==0) return base+5;
    if (fieldName[0]=='a' && strcmp(fieldName, "ack_nr")==0) return base+6;
    if (fieldName[0]=='d' && strcmp(fieldName, "delay")==0) return base+7;
    if (fieldName[0]=='m' && strcmp(fieldName, "mod")==0) return base+8;
    if (fieldName[0]=='l' && strcmp(fieldName, "lost")==0) return base+9;
    if (fieldName[0]=='d' && strcmp(fieldName, "duplicated")==0) return base+10;
    if (fieldName[0]=='e' && strcmp(fieldName, "error")==0) return base+11;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *MessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "string",
        "char",
        "int",
        "int",
        "double",
        "bool",
        "bool",
        "bool",
        "string",
    };
    return (field>=0 && field<12) ? fieldTypeStrings[field] : nullptr;
}

const char **MessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *MessageDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int MessageDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    Message *pp = (Message *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *MessageDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    Message *pp = (Message *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string MessageDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    Message *pp = (Message *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getNodeInd());
        case 1: return long2string(pp->getStartTime());
        case 2: return long2string(pp->getHeader());
        case 3: return oppstring2string(pp->getPayload());
        case 4: return long2string(pp->getTrailer());
        case 5: return long2string(pp->getFrame_type());
        case 6: return long2string(pp->getAck_nr());
        case 7: return double2string(pp->getDelay());
        case 8: return bool2string(pp->getMod());
        case 9: return bool2string(pp->getLost());
        case 10: return bool2string(pp->getDuplicated());
        case 11: return oppstring2string(pp->getError());
        default: return "";
    }
}

bool MessageDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    Message *pp = (Message *)object; (void)pp;
    switch (field) {
        case 0: pp->setNodeInd(string2long(value)); return true;
        case 1: pp->setStartTime(string2long(value)); return true;
        case 2: pp->setHeader(string2long(value)); return true;
        case 3: pp->setPayload((value)); return true;
        case 4: pp->setTrailer(string2long(value)); return true;
        case 5: pp->setFrame_type(string2long(value)); return true;
        case 6: pp->setAck_nr(string2long(value)); return true;
        case 7: pp->setDelay(string2double(value)); return true;
        case 8: pp->setMod(string2bool(value)); return true;
        case 9: pp->setLost(string2bool(value)); return true;
        case 10: pp->setDuplicated(string2bool(value)); return true;
        case 11: pp->setError((value)); return true;
        default: return false;
    }
}

const char *MessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *MessageDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    Message *pp = (Message *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}



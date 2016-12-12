#ifndef PARSER_H
#define PARSER_H
#include "bs_string.h"
#include "maths/bs_maths.h"
#include "maths/bs_quaternion.h"
#include "maths/bs_color.h"
#include "interface/iio.h"
#include "bs_ptr.h"
#include "bs_map.h"


 static const char* const aParserExtHeader = "+";
 static const char* const aParserSplitBegin = "[";
 static const char* const aParserSplitValue = ",";
 static const char* const aParserSplitEnd = "]";

 typedef enum
 {
     Parser_Bool,
     Parser_Char,
     Parser_Integer,
     Parser_Float,
     Parser_String,
     Parser_Vec2,
     Parser_Vec3,
     Parser_Vec4,
     Parser_Mat2,
     Parser_Mat3,
     Parser_Mat4,
     Parser_Quate,
     Parser_Color,
     Parser_Uuid,

     Parser_Count
 }eParserDataTypes;

 static const struct tParserObjectTypes{
    eParserDataTypes type;
    const char *text;
}
aParserObject[Parser_Count] =
{
   {Parser_Bool,     "Bool"},
   {Parser_Char,     "Char"},
   {Parser_Integer,  "Integer"},
   {Parser_Float,    "Float"},
   {Parser_String,   ""},
   {Parser_Vec2,     "Vec2"},
   {Parser_Vec3,     "Vec3"},
   {Parser_Vec4,     "Vec4"},
   {Parser_Mat2,     "Mat2"},
   {Parser_Mat3,     "Mat3"},
   {Parser_Mat4,     "Mat4"},
   {Parser_Quate,    "Quate"},
   {Parser_Color,    "Color"},
   {Parser_Uuid,     "Uuid"}
};

#define PP bs_ptr(bsPackage iparser)
typedef PP piparser;
#undef PP

 /*!
  * @brief The eParserTypes enum 解析器需要解析的类型
  * @enum eParserTypes
  */
typedef enum
{
    Parser_Text,
    Parser_Binary
}eParserTypes;

 /*!
  * @brief The tParserNodes struct 解析器获取的数据
  * @struct tParserNodes
  */
 typedef struct _tParserDatas_
 {
     friend class parser_node;

 public:
     ~_tParserDatas_(){}
     _tParserDatas_(){type = Parser_Count;}
     _tParserDatas_(const _tParserDatas_ &pd)
     {
         type = pd.type;
         data = pd.data;
     }
     _tParserDatas_ (bool d){
         data.append(uint8(d));
         type = Parser_Bool;
     }
     _tParserDatas_ (char d){
         data.append(uint8(d));
         type = Parser_Char;
     }
     _tParserDatas_ (longlong d){
         data.append((uint8*)&d, sizeof(longlong));
         type = Parser_Integer;
     }
     _tParserDatas_ (real d){
         data.append((uint8*)&d, sizeof(real));
         type = Parser_Float;
     }
     _tParserDatas_ (const bs_string &d){
         data.append((uint8*)d.data(), d.bytecount());
         type = Parser_String;
     }
     _tParserDatas_ (const vec2 &d){
         data.append((uint8*)&d, sizeof(d));
         type = Parser_Vec2;
     }
     _tParserDatas_ (const vec3 &d){
         data.append((uint8*)&d, sizeof(d));
         type = Parser_Vec3;
     }
     _tParserDatas_ (const vec4 &d){
         data.append((uint8*)&d, sizeof(d));
         type = Parser_Vec4;
     }
     _tParserDatas_ (const mat2 &d){
         data.append((uint8*)&d, sizeof(d));
         type = Parser_Mat2;
     }
     _tParserDatas_ (const mat3 &d){
         data.append((uint8*)&d, sizeof(d));
         type = Parser_Mat3;
     }
     _tParserDatas_ (const mat4 &d){
         data.append((uint8*)&d, sizeof(d));
         type = Parser_Mat4;
     }
     _tParserDatas_ (const quat &d){
         data.append((uint8*)&d, sizeof(d));
         type = Parser_Quate;
     }

     _tParserDatas_ (const bs_color &d){
         data.append((uint8*)&d, sizeof(d));
         type = Parser_Color;
     }

     _tParserDatas_ &operator = (const _tParserDatas_ &pd)
     {
         data = pd.data;
         type = pd.type;
         return *this;
     }

     template <typename T>
     T get()
     {
         if (type == Parser_String)
         {
             bs_string v = bs_string((bs_char*)data.data(), data.size()/sizeof(ushort));
             return *((T*)(&v));
         }
         else if (data.size() == sizeof(T))
             return *((T*)data.data());
         return T();
     }

 private:
     eParserDataTypes type;
     bs_array<uint8> data;
 }tParserDatas;

 /*!
  * @brief The tParserNodes struct 解析器获取的节点
  * @struct tParserNodes
  */
 typedef struct _tParserNodes_ :public bs_map<bs_string, struct _tParserRoots_>
 {

     _tParserNodes_():
     bs_map<bs_string, struct _tParserRoots_>()
     {}
     ~_tParserNodes_()
     {
         bs_map<bs_string, struct _tParserRoots_>::clear ();
     }
     typedef struct _NodeValue_ eValueTypes;
     bool op_begin()const;
     struct _NodeValue_ op_next(bool &ret)const;
 private:
     bs_map<bs_string, struct _tParserRoots_>::iterator op_value;
 }tParserNodes;

 /*!
  * @brief The tParserMemers struct 解析器获取的成员的值
  * @struct tParserMemers
  */
 typedef struct _MemerVaule_ :public bs_map<bs_string, tParserDatas>::iterator
 {

     _MemerVaule_(){}
     ~_MemerVaule_(){}
     _MemerVaule_(const bs_map<bs_string, tParserDatas>::iterator &it):
         bs_map<bs_string, tParserDatas>::iterator(it)
     {
     }
     tParserDatas data()const
     {
         return (*this).value();
     }
     tParserDatas &data()
     {
         return (*this).value();
     }
 }tMemerValues;
 /*!
  * @brief The tParserMemers struct 解析器获取的成员
  * @struct tParserMemers
  */
 typedef struct _tParserMemers_ :public bs_map<bs_string, tParserDatas>
 {

     _tParserMemers_():
         bs_map<bs_string, tParserDatas>()
     {}
     ~_tParserMemers_()
     {
         bs_map<bs_string, tParserDatas>::clear ();
     }
     typedef tMemerValues eValueTypes;
     bool op_begin()const
     {
         _tParserMemers_ *fack = (_tParserMemers_ *)this;
         fack->op_value = fack->begin();
         return fack->op_value != fack->end();
     }
     tMemerValues op_next(bool &ret)const
     {
         _tParserMemers_ *fack = (_tParserMemers_ *)this;
         if (fack->op_value == fack->end())
             ret = false;
         else
         {
             ret = true;
             return tMemerValues(fack->op_value++);
         }
         return fack->end();
     }

 private:
     bs_map<bs_string, tParserDatas>::iterator op_value;
 }tParserMemers;

 /*!
  * @brief The tParserRoots struct 解析器获取的跟
  * @struct tParserRoots
  */
 typedef struct _tParserRoots_ : public tParserNodes
 {
     friend class parser_node;

 public:
     _tParserRoots_():
         tParserNodes()
     {
         tParserNodes::clear ();
         memers.clear ();
     }
     tParserNodes &node()
     {
         return (*this);
     }
     const _tParserNodes_ &node()const
     {
         return (*this);
     }
     tParserMemers &memer()
     {
         return memers;
     }
     const tParserMemers &memer()const
     {
         return memers;
     }

     bool is_empty()const
     {
         return empty () & memers.empty ();
     }

 private:
     tParserMemers memers;
 }tParserRoots;

 /*!
  * @brief The tNodeValues struct 节点内部的值
  * @struct tNodeValues
  */
 typedef struct _NodeValue_ :public bs_map<bs_string, struct _tParserRoots_>::iterator
 {

     _NodeValue_(){}
     ~_NodeValue_(){}

     _NodeValue_(const bs_map<bs_string, struct _tParserRoots_>::iterator &it):
         bs_map<bs_string, struct _tParserRoots_>::iterator(it)
     {
     }
     tParserRoots data()const
     {
         return (*this).value();
     }
     tParserRoots &data()
     {
         return (*this).value();
     }

 }tNodeValues;

 /*!
  * @brief The iparser class 解析器接口
  * @class iparser
  * 1.解析器解析后的组成:
  *     |-> tParserRoots(根) --------------------------> tParserMemers(成员) --------------> tMemerValues(成员值)
  *     |     |--> tParserNodes(节点) --> tNodeValues(节点值)    |--> tParserDatas(数据)            |--> tParserDatas(数据)
  *     |<--------------|<--------------------|
  *
  *
  */
bsPackage iparser : public iio
{

    virtual ~iparser(){}

    virtual bool open(const bs_string &name, int mode = Io_ReadOnly)= 0;
    virtual bool open(int mode = Io_ReadOnly)= 0;
    virtual bool open(char *mem_data, int64 mem_len) = 0;
    virtual void close() = 0;

    virtual tParserRoots read(bool *ret = NULL) = 0;
    virtual bool write(const tParserRoots &root) = 0;

    virtual bs_string dump(const tParserRoots &root) = 0;

protected:
    virtual int64 read(void *, int64)const{return -1;}
    virtual int64 read(bs_byte &) const{return -1;}
    virtual int64 write(void *, int64)const{return -1;}
    virtual int64 write(const bs_byte &) const{return -1;}
    virtual bool seek(int64){return false;}

public:
    static piparser create(eParserTypes type = Parser_Text) ;
};


#define foreach_t(val_type, val, lva) val_type val;\
    for (bool bf = lva.op_begin(); val = lva.op_next(bf), bf == true;)

inline bool tParserNodes::op_begin() const
{
    tParserNodes *fack = (tParserNodes *)this;
    fack->op_value = fack->begin();
    return fack->op_value != fack->end();
}
inline struct _NodeValue_ tParserNodes::op_next(bool &ret)const
{
    tParserNodes *fack = (tParserNodes *)this;
    if (fack->op_value == fack->end())
        ret = false;
    else
    {
        ret = true;
        return tNodeValues(fack->op_value++);
    }
    return fack->end();
}

#endif // PARSER_H

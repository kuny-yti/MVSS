#include "bs_parser.h"
#include "bs_stack.h"
#include "bs_filesystem.h"
#include <yaml/yaml.h>

typedef enum
{
    Parser_Not,
    Parser_Key,
    Parser_Value,

    Parser_Start,
    Parser_Stop
}eParserStates;

class parser_node :public bs_map<bs_string, parser_node>
{
    friend class parser;
public:
        parser_node():
      bs_map<bs_string, parser_node>()
    {
    }
    parser_node(const bs_string &key):
        bs_map<bs_string, parser_node>()
    {
        tags = key;
    }
    virtual ~parser_node()
    {

    }

    bs_string maps(const bs_string &key)const
    {
        return memer.value (key);
    }
    bs_string &maps(const bs_string &key)
    {
        return memer[key];
    }
    void memer_append(const bs_string &key, const bs_string & val)
    {
        return memer.append (key, val);
    }

    bs_string tag()const
    {
        return tags;
    }
    void tag(const bs_string &k)
    {
        tags = k;
    }

    bs_string dump()
    {
        static int level = 1;
        bs_string out;
        bs_map<bs_string, bs_string>::iterator it;
        for (it = memer.begin (); it != memer.end (); ++it)
        {
            for (int i = 0; i < level; ++i)
                out.append (" ");
            out.append (it.key());
            out.append (" : ");
            out.append (it.value());
            out.append (" \n");
        }
        bs_map<bs_string, parser_node>::iterator itc;
        for (itc = this->begin (); itc != this->end (); ++itc)
        {
            for (int i = 0; i < level/2; ++i)
                out.append ("  ");
            level++;
            out.append (itc.key());
            out.append (" : ");
            out.append (" \n");
            parser_node &n = itc.value();
            out.append (n.dump());
            level--;
        }
        return out;
    }
    tParserRoots conver()
    {
        tParserRoots tem;
        bs_map<bs_string, bs_string>::iterator it;
        for (it = memer.begin (); it != memer.end (); ++it)
        {
            if (is_ext (it.value()))
            {
                eParserDataTypes tt = get_ext_type(it.value());
                bs_string_list val = get_ext_value (it.value());
                tParserDatas &pdats = tem.memers[it.key()];
                get_object(val, tt, pdats);
            }
            else
            {
                bs_list<bs_string> val;
                val.append (it.value());
                tParserDatas &pdats = tem.memers[it.key()];
                get_object(val, Parser_String, pdats);
            }
        }
        bs_map<bs_string, parser_node>::iterator itc;
        for (itc = this->begin (); itc != this->end (); ++itc)
        {
            parser_node &n = itc.value();
            tem.append (itc.key() , n.conver());
        }
        return tem;
    }
public:
    static parser_node conver(tParserRoots &on)
    {
        parser_node out;
        bs_map<bs_string, tParserDatas>::iterator it;
        for (it = on.memers.begin (); it != on.memers.end (); ++it)
        {
            if (it.value().type == Parser_String)
            {
                out.memer_append (it.key() ,it.value().get<bs_string>());
            }
            else
            {
                bs_string ext_str = get_ext_type(it.value());
                bs_string val = get_ext_value (it.value());

                if (is_ext(it.value()))
                {
                    bs_string peh(aParserExtHeader);
                    peh += ext_str;
                    peh += " ";
                    peh += aParserSplitBegin;
                    peh += val;
                    peh += aParserSplitEnd;
                    val = peh;
                }
                else
                {
                    ext_str += " ";
                    ext_str += aParserSplitBegin;
                    ext_str += val;
                    ext_str += aParserSplitEnd;
                    val = ext_str;
                }
                out.maps (it.key()) = val;
            }
        }
        bs_map<bs_string, tParserRoots>::iterator itc;
        for (itc = on.begin (); itc != on.end (); ++itc)
        {
            tParserRoots &n = itc.value();
            out.append (itc.key(), conver(n));
        }
        return out;
    }
private:
    static bs_string get_ext_type(const tParserDatas &d)
    {
        if ((d.type >= Parser_Bool) && (d.type < Parser_Count))
        {
            return aParserObject[d.type].text;
        }
        return bs_string();
    }
    static bs_string get_ext_value(tParserDatas &d)
    {
        bs_string out;
        switch ((int)d.type)
        {
        case Parser_Bool:
            out.form (d.get<bool>());
            return out;
        case Parser_Char:
            out.form (d.get<char>());
            return out;
        case Parser_Integer:
            out.form (d.get<longlong>());
            return out;
        case Parser_Float:
            out.form (d.get<real>());
            return out;
        case Parser_String:
            return d.get<bs_string>();
        case Parser_Vec2:
        {
            vec2 vb = d.get<vec2>();
            out.form (vb.x); out.append (aParserSplitValue);
            out.form (vb.y);
            return out;
        }
        case Parser_Vec3:
        {
            vec3 vb = d.get<vec3>();
            out.form (vb.x); out.append (aParserSplitValue);
            out.form (vb.y); out.append (aParserSplitValue);
            out.form (vb.z);
            return out;
        }
        case Parser_Vec4:
        {
            vec4 vb = d.get<vec4>();
            out.form (vb.x); out.append (aParserSplitValue);
            out.form (vb.y); out.append (aParserSplitValue);
            out.form (vb.z); out.append (aParserSplitValue);
            out.form (vb.w);
            return out;
        }
        case Parser_Mat2:
        {
            mat2 vb = d.get<mat2>();
            out.form (vb[0][0]); out.append (aParserSplitValue);
            out.form (vb[0][1]); out.append (aParserSplitValue);

            out.form (vb[1][0]); out.append (aParserSplitValue);
            out.form (vb[1][1]);
            return out;
        }
        case Parser_Mat3:
        {
            mat3 vb = d.get<mat3>();
            out.form (vb[0][0]); out.append (aParserSplitValue);
            out.form (vb[0][1]); out.append (aParserSplitValue);
            out.form (vb[0][2]); out.append (aParserSplitValue);

            out.form (vb[1][0]); out.append (aParserSplitValue);
            out.form (vb[1][1]); out.append (aParserSplitValue);
            out.form (vb[1][2]); out.append (aParserSplitValue);

            out.form (vb[2][0]); out.append (aParserSplitValue);
            out.form (vb[2][1]); out.append (aParserSplitValue);
            out.form (vb[2][2]);
            return out;
        }
        case Parser_Mat4:
        {
            mat4 vb = d.get<mat4>();
            out.form (vb[0][0]); out.append (aParserSplitValue);
            out.form (vb[0][1]); out.append (aParserSplitValue);
            out.form (vb[0][2]); out.append (aParserSplitValue);
            out.form (vb[0][3]); out.append (aParserSplitValue);

            out.form (vb[1][0]); out.append (aParserSplitValue);
            out.form (vb[1][1]); out.append (aParserSplitValue);
            out.form (vb[1][2]); out.append (aParserSplitValue);
            out.form (vb[1][3]); out.append (aParserSplitValue);

            out.form (vb[2][0]); out.append (aParserSplitValue);
            out.form (vb[2][1]); out.append (aParserSplitValue);
            out.form (vb[2][2]); out.append (aParserSplitValue);
            out.form (vb[2][3]); out.append (aParserSplitValue);

            out.form (vb[3][0]); out.append (aParserSplitValue);
            out.form (vb[3][1]); out.append (aParserSplitValue);
            out.form (vb[3][2]); out.append (aParserSplitValue);
            out.form (vb[3][3]);
            return out;
        }
        case Parser_Quate:
        {
            quat vb = d.get<quat>();
            out.form (vb.x); out.append (aParserSplitValue);
            out.form (vb.y); out.append (aParserSplitValue);
            out.form (vb.z); out.append (aParserSplitValue);
            out.form (vb.w);
            return out;
        }

        case Parser_Color:
        {
            bs_color vb = d.get<bs_color>();
            out.form (vb.r); out.append (aParserSplitValue);
            out.form (vb.g); out.append (aParserSplitValue);
            out.form (vb.b); out.append (aParserSplitValue);
            out.form (vb.a);
            return out;
        }
        default:
            break;
        }
        return bs_string();
    }
    static bool is_ext(const tParserDatas &d)
    {
        if ((d.type >= Parser_Bool) && (d.type < Parser_Count))
            if (d.type != Parser_String)
                return true;
        return false;
    }
private:
    static bool is_ext(const bs_string &val)
    {
        bs_string sv = val;
        if (sv.is_empty ())
            return false;
        return sv[0] == *aParserExtHeader;
    }
    static eParserDataTypes get_ext_type(const bs_string &val)
    {
        bs_string sv = val;
        if (sv.is_empty ())
            return Parser_Count;
        sv.remove (aParserExtHeader);
        bs_string_list spv(sv.split (aParserSplitBegin));
        if (spv.count () != 2)
            return Parser_Count;
        for (size_t i = 0; i < Parser_Count; ++i)
        {
            if (spv[0] == aParserObject[i].text)
                return aParserObject[i].type;
        }
        return Parser_Count;
    }
    static bs_string_list get_ext_value(const bs_string &val)
    {
        bs_string_list out;
        bs_string sv = val;
        if (sv.is_empty ())
            return out;
        bs_string_list spv (sv.split (aParserSplitBegin));
        if (spv.count () != 2)
            return out;

        spv[1].remove (aParserSplitEnd);
        out = spv[1].split(aParserSplitValue);
        return out;
    }
    static void get_object(bs_list<bs_string> &o, eParserDataTypes type, tParserDatas &dats)
    {
        switch ((int)type)
        {
        case Parser_Bool:
            if (o.count () == 1)
                dats = tParserDatas(o[0].to_bool ());
            break;
        case Parser_Char:
            if (o.count () == 1)
                dats = tParserDatas(o[0].to_char ());
            break;
        case Parser_Integer:
            if (o.count () == 1)
                dats = tParserDatas(o[0].to_longlong ());
            break;
        case Parser_Float:
            if (o.count () == 1)
                dats = tParserDatas(o[0].to_real ());
            break;
        case Parser_String:
            if (o.count () == 1)
                dats = tParserDatas(o[0]);
            break;
        case Parser_Vec2:
            if (o.count () == 2)
                dats = tParserDatas(vec2(o[0].to_float(), o[1].to_float()));
            break;
        case Parser_Vec3:
            if (o.count () == 3)
                dats = tParserDatas(vec3(o[0].to_float(), o[1].to_float(), o[2].to_float()));
            break;
        case Parser_Vec4:
            if (o.count () == 4)
                dats = tParserDatas(vec4(o[0].to_float(), o[1].to_float(), o[2].to_float(), o[3].to_float()));
            break;
        case Parser_Mat2:
            if (o.count () == 2*2)
                dats = tParserDatas(mat2(o[0].to_float(), o[1].to_float(), o[2].to_float(), o[3].to_float()));
            break;
        case Parser_Mat3:
            if (o.count () == 3*3)
                dats = tParserDatas(mat3(o[0].to_float(), o[1].to_float(), o[2].to_float(),
                        o[3].to_float(), o[4].to_float(), o[5].to_float(),
                        o[6].to_float(), o[7].to_float(), o[8].to_float()));
            break;
        case Parser_Mat4:
            if (o.count () == 4*4)
                dats = tParserDatas(mat4(o[0].to_float(), o[1].to_float(), o[2].to_float(), o[3].to_float(),
                        o[4].to_float(), o[5].to_float(), o[6].to_float(), o[7].to_float(),
                        o[8].to_float(), o[9].to_float(), o[10].to_float(), o[11].to_float(),
                        o[12].to_float(), o[13].to_float(), o[14].to_float(), o[15].to_float()));
            break;
        case Parser_Quate:
            if (o.count () == 4)
                dats = tParserDatas(quat(o[0].to_float(), o[1].to_float(), o[2].to_float(), o[3].to_float()));
            break;
        case Parser_Color:
            if (o.count () == 4)
            {
                int ir = o[0].to_float();
                int ig = o[1].to_float();
                int ib = o[2].to_float();
                int ia = o[3].to_float();
                if (ir > 1 || ig > 1 || ib > 1 || ia > 1)
                    dats = tParserDatas(bs_color(o[0].to_int(), o[1].to_int(),
                            o[2].to_int(), o[3].to_int()));
                else
                    dats = tParserDatas(bs_color(o[0].to_float(), o[1].to_float(),
                            o[2].to_float(), o[3].to_float()));
            }
            break;
        default:
            break;
        }
    }

private:
    bs_string tags;
    bs_map<bs_string, bs_string> memer;
};

class parser_text : public iparser
{
public:
    parser_text()
    {
        text = NULL;
        text_len = 0;
        input = NULL;

        io_eof = false;
        io_error = 0;
        io_open = false;
        io_operation_mode = 0;
        io_pos = 0;
        io_size = 0;
    }
    virtual ~parser_text()
    {

    }

    virtual bs_string errors()const{return bs_string();}
    virtual bool open(const bs_string &file, int mod = Io_ReadOnly)
    {
        if (is_open())
            return false;

        io_url = bs_url(file);
        return open(mod);
    }
    virtual bool open(int mod = Io_ReadOnly)
    {
        bs_fileinfo finfo(io_url.path());
        if (!finfo.is_exists())
            return false;
        if (io_operation_mode == Io_Raw || is_open())
            return true;
        if (mod == Io_ReadOnly)
        {
            yaml_parser_initialize(&_Parser);
            input = fopen((char*)finfo.files().to_utf8().data (), "rb");
            yaml_parser_set_input_file(&_Parser, input);
        }
        else if (mod == Io_WriteOnly)
            input = fopen((char*)finfo.files().to_utf8().data (), "wb");

        if (!input)
            return false;

        io_eof = false;
        io_error = 0;
        io_open = true;
        io_operation_mode = mod;
        io_pos = 0;
        io_size = finfo.size();

        return true;
    }
    virtual bool open(char *mem_data, int64 mem_len)
    {
        if (is_open ())
            close ();
        if (mem_data)
        {
            yaml_parser_initialize(&_Parser);
            text = mem_data;
            text_len = mem_len;
            yaml_parser_set_input_string(&_Parser, (uchar*)text, text_len);
        }
        else
            return false;

        io_eof = false;
        io_error = 0;
        io_open = true;
        io_operation_mode = Io_Raw;
        io_pos = 0;
        io_size = text_len;
        return true;
    }
    virtual void close()
    {
        if (is_open() && input)
            fclose(input);

        yaml_parser_delete(&_Parser);
        input = NULL;
        text = NULL;
        text_len = 0;
    }

    virtual tParserRoots read(bool *ret = NULL)
    {
        bool parser_ret = false;
        parser_node pn = parser(parser_ret);
        if (ret)
        {
            if (parser_ret)
                *ret = true;
            else
                *ret = false;
        }
        return pn.conver();
    }
    virtual bool write(const tParserRoots &root)
    {
        if ((text_len == -1) && io_operation_mode == Io_Raw)
            return false;

        tParserRoots rt = root;
        parser_node pn = parser_node::conver (rt);

        bs_string st = pn.dump ();
        bs_utf8 utf = st.to_utf8();
        if (io_operation_mode != Io_Raw)
            fwrite (utf.data (), utf.size(), 1, input);
        else if ((int64)utf.size() < text_len)
            bs_memory::copy_sse(text, utf.data (), utf.size());
        else
            return false;
        return true;
    }

    bs_string dump(const tParserRoots &root)
    {
        tParserRoots rt = root;
        parser_node pn = parser_node::conver (rt);
        return pn.dump ();
    }
private:
    parser_node parser(bool &ret) const
    {
        parser_text *pt = (parser_text*)this;

        parser_node root;
        eParserStates state ;
        yaml_token_t token;
        bs_string tkey;
        bs_stack<parser_node> stack;
        bs_stack<bs_string> ckey;
        int done = 0;

        while(!done)
        {
            if(!yaml_parser_scan(&pt->_Parser, &token))
            {
                ret = false;
                return root;
            }

            switch(token.type)
            {
            case YAML_KEY_TOKEN:
                state = Parser_Key;
                break;
            case YAML_VALUE_TOKEN:
                state = Parser_Value;
                break;

            case YAML_BLOCK_MAPPING_START_TOKEN:
            case YAML_FLOW_MAPPING_START_TOKEN:
            {
                if (state == Parser_Value)
                {
                    ckey.push ( tkey);
                    for ( size_t i = stack.count (); i < ckey.count (); ++i)
                        stack.push (parser_node());
                }
                else
                    stack.push (parser_node());
                state = Parser_Start;
                break;
            }
            case YAML_BLOCK_END_TOKEN:
            case YAML_FLOW_MAPPING_END_TOKEN:
            {
                if (stack.is_empty ())
                    break;
                parser_node pn = stack.pop ();
                if (stack.count ())
                {
                    parser_node &pm = stack.top ();
                    pm.append (ckey.pop (), pn);
                }
                else
                    root.append (ckey.pop (), pn);
                state = Parser_Stop;
                break;
            }
            case YAML_SCALAR_TOKEN:
            {
                if (state == Parser_Key)
                {
                    tkey = (const char*)token.data.scalar.value;
                }
                else if (state == Parser_Value)
                {
                    parser_node &pm = stack.top ();
                    pm.maps (tkey) = (const char*)token.data.scalar.value;
                }
                break;
            }
            default:
                break;
            }

            done = (token.type == YAML_STREAM_END_TOKEN);
            yaml_token_delete(&token);
        }
        ret = true;
        return root;
    }
private:
    yaml_parser_t _Parser;
    int64 text_len;
    FILE *input;
    char *text;

};

piparser iparser::create(eParserTypes type)
{
    if (type == Parser_Text)
        return bs_malloc(parser_text);
    return piparser();
}

#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<iterator>
#include<ctime>
#include<algorithm>
using namespace std;

class Sta;
class Attri;
class SlashState;
class TagRightEdge;
class TextRegion;
class EndTag;

enum FUN_TYPE{STA_TAG,TAG_NAME_BEFORE,TAG_NAME_MID,TAG_NAME_AFTER,
        ATTRI_STA,ATTRI_MID,ATTRI_VAL,ATTRI_EQUAL,ATTRI_VAL_MID,
        SLASH,
        RIGHT_EDGE,
        TEXT_STA,TEXT_MID,
        END_TAG_STA,END_TAG_STA2,END_TAGNAME_BEFORE,END_TAGNAME,END_TAGNAME_AFTER,
        OVER
};
typedef string::iterator siter;
typedef string::const_iterator csiter;
csiter End;
string ret;
inline bool isover(csiter& sta){
    return sta==End;
}
inline bool noover(csiter& sta){
    if(sta==End){
        cout<<"process error"<<endl;
        exit(0);
    }
    return true;
}
inline void delblank(csiter& pos){
    while(*pos==' '||*pos=='\n'){
        pos++;
    }
}
inline bool notblank(csiter& pos){
    return *pos!=' '&&*pos!='\n';
}
inline bool notchar(csiter& pos,char c){
    return *pos!=c;
}
inline void cutendblank(csiter& e){
    while(*e==' '||*e=='\n'){
        e--;
    }
}
inline void cutendblank(string& s){
    auto e=s.end()-1;
    while(*e==' '||*e=='\n'){
        e--;
    }
    e++;
    s.erase(e,s.end());
}
inline void ERROR(const string& errstr="ERROR"){
    cout<<errstr<<endl;
    cout<<ret.substr(ret.end()-ret.begin()-100,100)<<endl;
    exit(0);
}
inline csiter endblank(csiter end){
    while(*end==' '||*end=='\n'){
        end--;
    }
    return end;
}

class Sta{
    public:
        inline static FUN_TYPE staTag(string& s,csiter& pos);
        inline static FUN_TYPE tagNameBefore(string& s,csiter& pos);
        inline static FUN_TYPE tagNameMid(string& s,csiter& pos);
        inline static FUN_TYPE tagNameAfter(string& s,csiter& pos);
};
class Attri{
    public:
        inline static FUN_TYPE AttriSta(string& s,csiter& pos);
        inline static FUN_TYPE AttriMid(string& s,csiter& pos);
        inline static FUN_TYPE AttriVal(string& s,csiter& pos);
        inline static FUN_TYPE AttriEqual(string& s,csiter& pos);
        inline static FUN_TYPE AttriValMid(string& s,csiter& pos);
};
class SlashState{
    public:
        inline static FUN_TYPE slash(string& s,csiter& pos);
};
class TagRightEdge{
    public:
        inline static FUN_TYPE rightEdge(string& s,csiter& pos);
};
class TextRegion{
    public:
        inline static FUN_TYPE TextSta(string& s,csiter& pos);
        inline static FUN_TYPE TextMid(string& s,csiter& pos);
};
class EndTag{
    public:
        inline static FUN_TYPE EndTagSta(string& s,csiter& pos);
        inline static FUN_TYPE EndTagSta2(string& s,csiter& pos);
        inline static FUN_TYPE EndTagNameBefore(string& s,csiter& pos);
        inline static FUN_TYPE EndTagName(string& s,csiter& pos);
        inline static FUN_TYPE EndTagNameAfter(string& s,csiter& pos);
};

FUN_TYPE Sta::staTag(string& s,csiter& pos){
    if(isover(pos)){
        cout<<"process over"<<endl;
        return OVER;
    }
    delblank(pos);
    if(*pos=='<'){
        pos++;
        s+="<";
        noover(pos);
        delblank(pos);
        if(*pos!='/'){
            return TAG_NAME_BEFORE;
        }else{
            return END_TAG_STA2;
        }
    }else{
        ERROR();
    }
    return OVER;
}
FUN_TYPE Sta::tagNameBefore(string& s,csiter& pos){
    noover(pos);
    delblank(pos);
    return TAG_NAME_MID;
}
FUN_TYPE Sta::tagNameMid(string& s,csiter& pos){
    while(noover(pos)&&notblank(pos)&&*pos!='>'){
        s+=*pos;
        pos++;
    }
    return TAG_NAME_AFTER;
}
FUN_TYPE Sta::tagNameAfter(string& s,csiter& pos){
    delblank(pos);
    if(*pos=='>'){
        s+=">";
        pos++;
        return RIGHT_EDGE;
    }else if(*pos=='/'){
        s+="/";
        pos++;
        return SLASH;
    }else{
        return ATTRI_STA;
    }
}

FUN_TYPE SlashState::slash(string& s,csiter& pos){
    noover(pos);
    delblank(pos);
    if(noover(pos)&&*pos=='>'){
        s+=">";
        pos++;
        return STA_TAG;
    }else{
        ERROR();
    }
    return OVER;
}

FUN_TYPE TagRightEdge::rightEdge(string& s,csiter& pos){
    noover(pos);
    delblank(pos);
    if(*pos=='<'){
        s+="<";
        pos++;
        if(noover(pos)&&*pos=='/'){
            return END_TAG_STA2;
        }else{
            return TAG_NAME_BEFORE;
        }
    }else{
        return TEXT_STA;
    }
}

FUN_TYPE TextRegion::TextSta(string& s,csiter& pos){
    delblank(pos);
    return TEXT_MID;
}
FUN_TYPE TextRegion::TextMid(string& s,csiter& pos){
    while(noover(pos)){
        if(*pos!='<'){
            s+=*pos;
            pos++;
        }else{
            auto it=endblank(s.end()-1)+1;
            s.erase(it,s.end());
            return END_TAG_STA;
        }
    }
    return OVER;
}

FUN_TYPE Attri::AttriSta(string& s,csiter& pos){
    noover(pos);
    delblank(pos);
    if(*pos=='>'){
        s+=">";
        pos++;
        return RIGHT_EDGE;
    }else if(*pos=='/'){
        s+="/";
        pos++;
        return SLASH;
    }else{
        s+=" ";
        return ATTRI_MID;
    }
}
FUN_TYPE Attri::AttriMid(string& s,csiter& pos){
    while(noover(pos)&&notblank(pos)&&*pos!='='){
        s+=*pos;
        pos++;
    }
    return ATTRI_EQUAL;
}
FUN_TYPE Attri::AttriEqual(string& s,csiter& pos){
    noover(pos);
    delblank(pos);
    if(noover(pos)&&*pos=='='){
        pos++;
        s+="=";
        return ATTRI_VAL;
    }else{
        ERROR();
    }
    return OVER;
}
FUN_TYPE Attri::AttriVal(string& s,csiter& pos){
    noover(pos);
    delblank(pos);
    if(noover(pos)&&*pos=='"'){
        pos++;
        s+="\"";
        return ATTRI_VAL_MID;
    }else{
        ERROR();
    }
    return OVER;
}
FUN_TYPE Attri::AttriValMid(string& s,csiter& pos){
    while(noover(pos)&&*pos!='"'){
        s+=*pos;
        pos++;
    }
    s+="\"";
    pos++;
    return ATTRI_STA;
}

FUN_TYPE EndTag::EndTagSta(string& s,csiter& pos){
    delblank(pos);
    if(*pos=='<'){
        s+="<";
        pos++;
        return END_TAG_STA2;
    }else{
        ERROR();
    }
    return OVER;
}
FUN_TYPE EndTag::EndTagSta2(string& s,csiter& pos){
    noover(pos);
    delblank(pos);
    if(*pos=='/'){
        s+="/";
        pos++;
        return END_TAGNAME_BEFORE;
    }else{
        ERROR();
    }
    return OVER;
}
FUN_TYPE EndTag::EndTagNameBefore(string& s,csiter& pos){
    noover(pos);
    delblank(pos);
    if(*pos=='>'){
        ERROR();
    }
    return END_TAGNAME;
}
FUN_TYPE EndTag::EndTagName(string& s,csiter& pos){
    while(noover(pos)&&*pos!=' '){
        if(*pos!='>'){
            s+=*pos;
            pos++;
        }else{
            s+=">";
            pos++;
            return STA_TAG;
        }
    }
    return END_TAGNAME_AFTER;
}
FUN_TYPE EndTag::EndTagNameAfter(string& s,csiter& pos){
    delblank(pos);
    if(*pos=='>'){
        s+=">";
        pos++;
        return STA_TAG;
    }else{
        ERROR();
    }
    return OVER;
}

void divid(){
    string str(60,'-');
    cout<<str<<endl;
}

void execute(string& s,csiter& pos){
    FUN_TYPE i;
    l1:
    i=Sta::staTag(s,pos);
    goto pick_route;
    l2:
    i=Sta::tagNameBefore(s,pos);
    goto pick_route;
    l3:
    i=Sta::tagNameMid(s,pos);
    goto pick_route;
    l4:
    i=Sta::tagNameAfter(s,pos);
    goto pick_route;
    l5:
    i=Attri::AttriSta(s,pos);
    goto pick_route;
    l6:
    i=Attri::AttriMid(s,pos);
    goto pick_route;
    l7:
    i=Attri::AttriVal(s,pos);
    goto pick_route;
    l8:
    i=Attri::AttriEqual(s,pos);
    goto pick_route;
    l9:
    i=Attri::AttriValMid(s,pos);
    goto pick_route;
    l10:
    i=SlashState::slash(s,pos);
    goto pick_route;
    l11:
    i=TagRightEdge::rightEdge(s,pos);
    goto pick_route;
    l12:
    i=TextRegion::TextSta(s,pos);
    goto pick_route;
    l13:
    i=TextRegion::TextMid(s,pos);
    goto pick_route;
    l14:
    i=EndTag::EndTagSta(s,pos);
    goto pick_route;
    l15:
    i=EndTag::EndTagSta2(s,pos);
    goto pick_route;
    l16:
    i=EndTag::EndTagNameBefore(s,pos);
    goto pick_route;
    l17:
    i=EndTag::EndTagName(s,pos);
    goto pick_route;
    l18:
    i=EndTag::EndTagNameAfter(s,pos);
    goto pick_route;
    pick_route:
    switch(i){
        case STA_TAG:{
            goto l1;
        }
        case TAG_NAME_BEFORE:{
            goto l2;
        }
        case TAG_NAME_MID:{
            goto l3;
        }
        case TAG_NAME_AFTER:{
            goto l4;
        }
        case ATTRI_STA:{
            goto l5;
        }
        case ATTRI_MID:{
            goto l6;
        }
        case ATTRI_VAL:{
            goto l7;
        }
        case ATTRI_EQUAL:{
            goto l8;
        }
        case ATTRI_VAL_MID:{
            goto l9;
        }
        case SLASH:{
            goto l10;
        }
        case RIGHT_EDGE:{
            goto l11;
        }
        case TEXT_STA:{
            goto l12;
        }
        case TEXT_MID:{
            goto l13;
        }
        case END_TAG_STA:{
            goto l14;
        }
        case END_TAG_STA2:{
            goto l15;
        }
        case END_TAGNAME_BEFORE:{
            goto l16;
        }
        case END_TAGNAME:{
            goto l17;
        }
        case END_TAGNAME_AFTER:{
            goto l18;
        }
        case OVER:{
            goto process_over;
        }
    }
    process_over:
    cout<<"execute over"<<endl;
}
int main(){
    ifstream ifs("C:\\Users\\tomst\\Desktop\\xml2\\in.xml");
    ifs.unsetf(ios::skipws);
    string str;
    copy(istream_iterator<char>(ifs),istream_iterator<char>(),back_insert_iterator(str));
    cutendblank(str);
    End=str.end(); //set End of str
    divid();
    cout<<"XmlStrSize: "<<str.size()<<endl;
    divid();
    csiter pos=str.begin();
    auto statime=clock();
    execute(ret,pos);
    cout<<"time cost is: "<<clock()-statime<<" ms"<<endl;
    cout<<"process size: "<<ret.size()<<endl;
    divid();
    auto showlen=min(string::size_type(150),ret.size());
    cout<<ret.substr(ret.end()-ret.begin()-showlen,showlen)<<endl;
    return 0;
}
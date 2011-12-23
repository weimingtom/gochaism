/**
 * seibun.js for LimeChat
 * いわゆる「成分解析」のクローン
 * 
 * スクリプトを動作させたいチャンネルで
 *  seibun/start
 * と発言するか、あるいは
 *  seibun/start #channel1 #channel2 ...
 * のように発言することで、スクリプトが有効になります。
 * 同様にexitでスクリプトを無効にできます。
 * ※スクリプト動作主の発言のみを受け付けます
 * 
 * あとは
 *  .seibun <名詞>
 * で成分解析が可能です。
 * 
 * コード自体はインターネットの大海原から拾ってきたものです。
 * この場を借りてお礼申し上げます。みみずさんありがとう。
 * 
 * @author  gocha
 * @version 2009-01-08
 */

var scriptName = "seibun";

//-- [ global / initialize ] ---------------------------------------------------

function event::onLoad()
{
    loadChList();
}

//-- [ customizable routines ] -------------------------------------------------

function textOut(target, text)
{
    send(target, text);
}

function errorOut(target, text)
{
    log(text);
}

function debugOut(target, text)
{
    log(text);
}

//------------------------------------------------------------------------------

var chList = new Array();
var chListFile = scriptName + "_chlist.txt";

function loadList(list, filename, defaultStat)
{
    for (var i in list) {
        list[i.toLowerCase()] = defaultStat;
    }

    var file = openFile(filename);
    if (file) {
        var line;
        while( (line = file.readLine()) != null) {
            list[ line.toLowerCase() ] = !defaultStat;
        }
        file.close();
    }
}

function saveList(list, filename, defaultStat)
{
    var file = openFile(filename, false);
    if (file) {
        for (var i in list) {
            if (list[i.toLowerCase()] != defaultStat)
                file.writeLine(i);
        }
        file.truncate();
        file.close();
    }
}

function loadChList()
{
    loadList(chList, chListFile, false);
}

function saveChList()
{
    saveList(chList, chListFile, false);
}

function scriptCfgSetStat(prefix, target, cmd, arg, newStat)
{
    if (arg.length == 0) {
        chList[target.toLowerCase()] = newStat;
    }
    else {
        for (i = 0; i < arg.length; i++) {
            chList[arg[i].toLowerCase()] = newStat;
        }
    }
    saveChList();
    textOut(target, scriptName + (newStat ? ' started' : ' stopped'));
}

function scriptCfgStart(prefix, target, cmd, arg)
{
    scriptCfgSetStat(prefix, target, cmd, arg, true);
}

function scriptCfgEnd(prefix, target, cmd, arg)
{
    scriptCfgSetStat(prefix, target, cmd, arg, false);
}

function scriptCfgStat(prefix, target, cmd, arg)
{
    textOut(target, scriptName + ' is ' + (
        chList[target.toLowerCase()] ? 'running' : 'stopped'));
}

function scriptCfgUpdate(prefix, target, cmd, arg)
{
    loadChList();
    textOut(target, scriptName + ' reloaded settings');
}

function scriptCfgCmd(prefix, target, cmd, arg)
{
    if (cmd.match(/^start|^run/))
        scriptCfgStart(prefix, target, cmd, arg);
    else if (cmd.match(/^end|^exit|^quit/))
        scriptCfgEnd(prefix, target, cmd, arg);
    else if (cmd.match(/^stat/))
        scriptCfgStat(prefix, target, cmd, arg);
    else if (cmd.match(/^update/))
        scriptCfgUpdate(prefix, target, cmd, arg);
    else
        textOut(target, 'Unknown command: ' + cmd);
}

//-- [ common functions ] ------------------------------------------------------

function trim(str)
{
    return str.replace(/^\s+/, '').replace(/\s+$/, '');
}

function stripTextDecoration(str)
{
    // \x02: <bold> \x03aa,bb: <color aa,bb>
    // \x0f: <stop> \x16: <reverse> \x1f: <underline>
    return str.replace(/[\x02\x0f\x16\x1f]|\x03\d\d(?:,\d\d)?/g, '');
}

var entity_list = {"ne":8800,"le":8804,"para":182,"xi":958,"darr":8595,"nu":957,"oacute":243,"Uacute":218,"omega":969,"prime":8242,"pound":163,"igrave":236,"thorn":254,"forall":8704,"emsp":8195,"lowast":8727,"brvbar":166,"alefsym":8501,"nbsp":160,"delta":948,"clubs":9827,"lArr":8656,"Omega":937,"quot":34,"Auml":196,"cedil":184,"and":8743,"plusmn":177,"ge":8805,"raquo":187,"uml":168,"equiv":8801,"laquo":171,"Epsilon":917,"rdquo":8221,"divide":247,"fnof":402,"chi":967,"Dagger":8225,"iacute":237,"rceil":8969,"sigma":963,"Oslash":216,"acute":180,"frac34":190,"upsih":978,"lrm":8206,"Scaron":352,"part":8706,"exist":8707,"nabla":8711,"image":8465,"prop":8733,"omicron":959,"zwj":8205,"gt":62,"aacute":225,"Yuml":376,"Yacute":221,"weierp":8472,"rsquo":8217,"otimes":8855,"kappa":954,"thetasym":977,"harr":8596,"Ouml":214,"Iota":921,"ograve":242,"sdot":8901,"copy":169,"oplus":8853,"acirc":226,"sup":8835,"zeta":950,"Iacute":205,"Oacute":211,"crarr":8629,"Nu":925,"bdquo":8222,"lsquo":8216,"Beta":914,"eacute":233,"egrave":232,"lceil":8968,"Kappa":922,"piv":982,"Ccedil":199,"ldquo":8220,"Xi":926,"cent":162,"uarr":8593,"hellip":8230,"Aacute":193,"ensp":8194,"sect":167,"Ugrave":217,"aelig":230,"ordf":170,"curren":164,"sbquo":8218,"macr":175,"Phi":934,"Eta":919,"rho":961,"Omicron":927,"sup2":178,"euro":8364,"aring":229,"Theta":920,"mdash":8212,"uuml":252,"otilde":245,"eta":951,"uacute":250,"rArr":8658,"nsub":8836,"agrave":224,"notin":8713,"Psi":936,"ndash":8211,"Ocirc":212,"sube":8838,"szlig":223,"micro":181,"not":172,"sup1":185,"middot":183,"iota":953,"ecirc":234,"lsaquo":8249,"thinsp":8201,"sum":8721,"ntilde":241,"scaron":353,"cap":8745,"atilde":227,"lang":9001,"isin":8712,"gamma":947,"Euml":203,"ang":8736,"upsilon":965,"Ntilde":209,"hearts":9829,"Tau":932,"Alpha":913,"spades":9824,"THORN":222,"dagger":8224,"int":8747,"lambda":955,"Eacute":201,"Uuml":220,"infin":8734,"Aring":197,"rlm":8207,"ugrave":249,"Egrave":200,"Acirc":194,"ETH":208,"oslash":248,"rsaquo":8250,"alpha":945,"Ograve":210,"Prime":8243,"mu":956,"ni":8715,"real":8476,"bull":8226,"beta":946,"icirc":238,"eth":240,"prod":8719,"larr":8592,"ordm":186,"perp":8869,"Gamma":915,"Pi":928,"reg":174,"ucirc":251,"psi":968,"tilde":732,"asymp":8776,"zwnj":8204,"Agrave":192,"Delta":916,"deg":176,"AElig":198,"times":215,"sim":8764,"Mu":924,"Otilde":213,"uArr":8657,"circ":710,"theta":952,"Rho":929,"sup3":179,"diams":9830,"tau":964,"Chi":935,"frac14":188,"oelig":339,"shy":173,"or":8744,"dArr":8659,"phi":966,"Lambda":923,"iuml":239,"rfloor":8971,"iexcl":161,"cong":8773,"ccedil":231,"Icirc":206,"frac12":189,"loz":9674,"rarr":8594,"cup":8746,"radic":8730,"frasl":8260,"euml":235,"OElig":338,"hArr":8660,"Atilde":195,"lt":60,"Upsilon":933,"there4":8756,"ouml":246,"oline":8254,"Ecirc":202,"yacute":253,"amp":38,"auml":228,"sigmaf":962,"permil":8240,"iquest":191,"empty":8709,"pi":960,"Ucirc":219,"supe":8839,"Igrave":204,"yen":165,"rang":9002,"trade":8482,"lfloor":8970,"minus":8722,"Zeta":918,"sub":8834,"epsilon":949,"Sigma":931,"yuml":255,"Iuml":207,"ocirc":244};
function decodeEntities(str)
{
    return str.replace(/&#(\d+);?|&#x([0-9a-fA-F]+);?|&(AElig|Aacute|Acirc|Agrave|Alpha|Aring|Atilde|Auml|Beta|Ccedil|Chi|Dagger|Delta|ETH|Eacute|Ecirc|Egrave|Epsilon|Eta|Euml|Gamma|Iacute|Icirc|Igrave|Iota|Iuml|Kappa|Lambda|Mu|Ntilde|Nu|OElig|Oacute|Ocirc|Ograve|Omega|Omicron|Oslash|Otilde|Ouml|Phi|Pi|Prime|Psi|Rho|Scaron|Sigma|THORN|Tau|Theta|Uacute|Ucirc|Ugrave|Upsilon|Uuml|Xi|Yacute|Yuml|Zeta|aacute|acirc|acute|aelig|agrave|alefsym|alpha|amp|and|ang|aring|asymp|atilde|auml|bdquo|beta|brvbar|bull|cap|ccedil|cedil|cent|chi|circ|clubs|cong|copy|crarr|cup|curren|dArr|dagger|darr|deg|delta|diams|divide|eacute|ecirc|egrave|empty|emsp|ensp|epsilon|equiv|eta|eth|euml|euro|exist|fnof|forall|frac12|frac14|frac34|frasl|gamma|ge|gt|hArr|harr|hearts|hellip|iacute|icirc|iexcl|igrave|image|infin|int|iota|iquest|isin|iuml|kappa|lArr|lambda|lang|laquo|larr|lceil|ldquo|le|lfloor|lowast|loz|lrm|lsaquo|lsquo|lt|macr|mdash|micro|middot|minus|mu|nabla|nbsp|ndash|ne|ni|not|notin|nsub|ntilde|nu|oacute|ocirc|oelig|ograve|oline|omega|omicron|oplus|or|ordf|ordm|oslash|otilde|otimes|ouml|para|part|permil|perp|phi|pi|piv|plusmn|pound|prime|prod|prop|psi|quot|rArr|radic|rang|raquo|rarr|rceil|rdquo|real|reg|rfloor|rho|rlm|rsaquo|rsquo|sbquo|scaron|sdot|sect|shy|sigma|sigmaf|sim|spades|sub|sube|sum|sup|sup1|sup2|sup3|supe|szlig|tau|there4|theta|thetasym|thinsp|thorn|tilde|times|trade|uArr|uacute|uarr|ucirc|ugrave|uml|upsih|upsilon|uuml|weierp|xi|yacute|yen|yuml|zeta|zwj|zwnj);?/g,function(str, int, hex, ent){
        if (int) {
            return String.fromCharCode(int);
        } else if (hex) {
            return String.fromCharCode(parseInt("0x"+hex));
        } else if (entity_list[ent]) {
            return String.fromCharCode(entity_list[ent]);
        } else {
            return "&" + ent;
        }
    });
}

function XMLHttpRequest()
{
    try {
        return new ActiveXObject("Msxml2.ServerXMLHTTP.6.0");
    } catch(e) {}
    try {
        return new ActiveXObject("Msxml2.ServerXMLHTTP.5.0");
    } catch(e) {}
    try {
        return new ActiveXObject("Msxml2.ServerXMLHTTP.4.0");
    } catch(e) {}
    try {
        return new ActiveXObject("Msxml2.ServerXMLHTTP.3.0");
    } catch(e) {}
    try {
        return new ActiveXObject("Msxml2.ServerXMLHTTP");
    } catch(e) {}
}

//-- [ subroutine ] ------------------------------------------------------------

//
// Escape Codec Library: ecl.js (Ver.041208)
//
// Copyright (C) http://nurucom-archives.hp.infoseek.co.jp/digital/
//

EscapeSJIS=function(str){
    return str.replace(/[^*+.-9A-Z_a-z-]/g,function(s){
        var c=s.charCodeAt(0),m;
        return c<128?(c<16?"%0":"%")+c.toString(16).toUpperCase():65376<c&&c<65440?"%"+(c-65216).toString(16).toUpperCase():(c=JCT11280.indexOf(s))<0?"%81E":"%"+((m=((c<8272?c:(c=JCT11280.lastIndexOf(s)))-(c%=188))/188)<31?m+129:m+193).toString(16).toUpperCase()+(64<(c+=c<63?64:65)&&c<91||95==c||96<c&&c<123?String.fromCharCode(c):"%"+c.toString(16).toUpperCase())
    })
};

UnescapeSJIS=function(str){
    return str.replace(/%(8[1-9A-F]|[9E][0-9A-F]|F[0-9A-C])(%[4-689A-F][0-9A-F]|%7[0-9A-E]|[@-~])|%([0-7][0-9A-F]|A[1-9A-F]|[B-D][0-9A-F])/ig,function(s){
        var c=parseInt(s.substring(1,3),16),l=s.length;
        return 3==l?String.fromCharCode(c<160?c:c+65216):JCT11280.charAt((c<160?c-129:c-193)*188+(4==l?s.charCodeAt(3)-64:(c=parseInt(s.substring(4),16))<127?c-64:c-65))
    })
};

EscapeEUCJP=function(str){
    return str.replace(/[^*+.-9A-Z_a-z-]/g,function(s){
        var c=s.charCodeAt(0);
        return (c<128?(c<16?"%0":"%")+c.toString(16):65376<c&&c<65440?"%8E%"+(c-65216).toString(16):(c=JCT8836.indexOf(s))<0?"%A1%A6":"%"+((c-(c%=94))/94+161).toString(16)+"%"+(c+161).toString(16)).toUpperCase()
    })
};

UnescapeEUCJP=function(str){
    return str.replace(/(%A[1-9A-F]|%[B-E][0-9A-F]|%F[0-9A-E]){2}|%8E%(A[1-9A-F]|[B-D][0-9A-F])|%[0-7][0-9A-F]/ig,function(s){
        var c=parseInt(s.substring(1),16);
        return c<161?String.fromCharCode(c<128?c:parseInt(s.substring(4),16)+65216):JCT8836.charAt((c-161)*94+parseInt(s.substring(4),16)-161)
    })
};

EscapeJIS7=function(str){
    var u=String.fromCharCode,ri=u(92,120,48,48,45,92,120,55,70),rj=u(65377,45,65439,93,43),
    H=function(c){
        return 41<c&&c<58&&44!=c||64<c&&c<91||95==c||96<c&&c<123?u(c):"%"+c.toString(16).toUpperCase()
    },
    I=function(s){
        var c=s.charCodeAt(0);
        return (c<16?"%0":"%")+c.toString(16).toUpperCase()
    },
    rI=new RegExp;rI.compile("[^*+.-9A-Z_a-z-]","g");
    return ("g"+str+"g").replace(RegExp("["+ri+"]+","g"),function(s){
        return "%1B%28B"+s.replace(rI,I)
    }).replace(RegExp("["+rj,"g"),function(s){
        var c,i=0,t="%1B%28I";while(c=s.charCodeAt(i++))t+=H(c-65344);return t
    }).replace(RegExp("[^"+ri+rj,"g"),function(s){
        var a,c,i=0,t="%1B%24B";while(a=s.charAt(i++))t+=(c=JCT8836.indexOf(a))<0?"%21%26":H((c-(c%=94))/94+33)+H(c+33);return t
    }).slice(8,-1)
};

UnescapeJIS7=function(str){
    var i=0,p,q,s="",u=String.fromCharCode,
    P=("%28B"+str.replace(/%49/g,"I").replace(/%1B%24%4[02]|%1B%24@/ig,"%1B%24B")).split(/%1B/i),
    I=function(s){
        return u(parseInt(s.substring(1),16))
    },
    J=function(s){
        return u((3==s.length?parseInt(s.substring(1),16):s.charCodeAt(0))+65344)
    },
    K=function(s){
        var l=s.length;
        return JCT8836.charAt(4<l?(parseInt(s.substring(1),16)-33)*94+parseInt(s.substring(4),16)-33:2<l?(37==(l=s.charCodeAt(0))?(parseInt(s.substring(1,3),16)-33)*94+s.charCodeAt(3):(l-33)*94+parseInt(s.substring(2),16))-33:(s.charCodeAt(0)-33)*94+s.charCodeAt(1)-33)
    },
    rI=new RegExp,rJ=new RegExp,rK=new RegExp;
    rI.compile("%[0-7][0-9A-F]","ig");rJ.compile("(%2[1-9A-F]|%[3-5][0-9A-F])|[!-_]","ig");
    rK.compile("(%2[1-9A-F]|%[3-6][0-9A-F]|%7[0-9A-E]){2}|(%2[1-9A-F]|%[3-6][0-9A-F]|%7[0-9A-E])[!-~]|[!-~](%2[1-9A-F]|%[3-6][0-9A-F]|%7[0-9A-E])|[!-~]{2}","ig");
    while(p=P[i++])s+="%24B"==(q=p.substring(0,4))?p.substring(4).replace(rK,K):"%28I"==q?p.substring(4).replace(rJ,J):p.replace(rI,I).substring(2);
    return s
};

EscapeJIS8=function(str){
    var u=String.fromCharCode,r=u(92,120,48,48,45,92,120,55,70,65377,45,65439,93,43),
    H=function(c){
        return 41<c&&c<58&&44!=c||64<c&&c<91||95==c||96<c&&c<123?u(c):"%"+c.toString(16).toUpperCase()
    },
    I=function(s){
        var c=s.charCodeAt(0);
        return (c<16?"%0":"%")+(c<128?c:c-65216).toString(16).toUpperCase()
    },
    rI=new RegExp;rI.compile("[^*+.-9A-Z_a-z-]","g");
    return ("g"+str+"g").replace(RegExp("["+r,"g"),function(s){
        return "%1B%28B"+s.replace(rI,I)
    }).replace(RegExp("[^"+r,"g"),function(s){
        var a,c,i=0,t="%1B%24B";while(a=s.charAt(i++))t+=(c=JCT8836.indexOf(a))<0?"%21%26":H((c-(c%=94))/94+33)+H(c+33);return t
    }).slice(8,-1)
};

UnescapeJIS8=function(str){
    var i=0,p,s="",
    P=("%28B"+str.replace(/%1B%24%4[02]|%1B%24@/ig,"%1B%24B")).split(/%1B/i),
    I=function(s){
        var c=parseInt(s.substring(1),16);
        return String.fromCharCode(c<128?c:c+65216)
    },
    K=function(s){
        var l=s.length;
        return JCT8836.charAt(4<l?(parseInt(s.substring(1),16)-33)*94+parseInt(s.substring(4),16)-33:2<l?(37==(l=s.charCodeAt(0))?(parseInt(s.substring(1,3),16)-33)*94+s.charCodeAt(3):(l-33)*94+parseInt(s.substring(2),16))-33:(s.charCodeAt(0)-33)*94+s.charCodeAt(1)-33)
    },
    rI=new RegExp,rK=new RegExp;
    rI.compile("%([0-7][0-9A-F]|A[1-9A-F]|[B-D][0-9A-F])","ig");
    rK.compile("(%2[1-9A-F]|%[3-6][0-9A-F]|%7[0-9A-E]){2}|(%2[1-9A-F]|%[3-6][0-9A-F]|%7[0-9A-E])[!-~]|[!-~](%2[1-9A-F]|%[3-6][0-9A-F]|%7[0-9A-E])|[!-~]{2}","ig");
    while(p=P[i++])s+="%24B"==p.substring(0,4)?p.substring(4).replace(rK,K):p.replace(rI,I).substring(2);
    return s
};

EscapeUnicode=function(str){
    return str.replace(/[^*+.-9A-Z_a-z-]/g,function(s){
        var c=s.charCodeAt(0);
        return (c<16?"%0":c<256?"%":c<4096?"%u0":"%u")+c.toString(16).toUpperCase()
    })
};

UnescapeUnicode=function(str){
    return str.replace(/%u[0-9A-F]{4}|%[0-9A-F]{2}/ig,function(s){
        return String.fromCharCode("0x"+s.substring(s.length/3))
    })
};

EscapeUTF7=function(str){
    var B="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/".split(""),
    E=function(s){
        var c=s.charCodeAt(0);
        return B[c>>10]+B[c>>4&63]+B[(c&15)<<2|(c=s.charCodeAt(1))>>14]+(0<=c?B[c>>8&63]+B[c>>2&63]+B[(c&3)<<4|(c=s.charCodeAt(2))>>12]+(0<=c?B[c>>6&63]+B[c&63]:""):"")
    },
    re=new RegExp;re.compile("[^+]{1,3}","g");
    return (str+"g").replace(/[^*+.-9A-Z_a-z-]+[*+.-9A-Z_a-z-]|[+]/g,function(s){
        if("+"==s)return "+-";
        var l=s.length-1,w=s.charAt(l);
        return "+"+s.substring(0,l).replace(re,E)+("+"==w?"-+-":"*"==w||"."==w||"_"==w?w:"-"+w)
    }).slice(0,-1)
};

UnescapeUTF7=function(str){
    var i=0,B={};
    while(i<64)B["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/".charAt(i)]=i++;
    return str.replace(RegExp("[+][+/-9A-Za-z]*-?","g"),function(s){
        if("+-"==s)return "+";
        var b=B[s.charAt(1)],c,i=1,t="";
        while(0<=b){
            if((c=i&7)<6)c=c<3?b<<10|B[s.charAt(++i)]<<4|(b=B[s.charAt(++i)])>>2:(b&3)<<14|B[s.charAt(++i)]<<8|B[s.charAt(++i)]<<2|(b=B[s.charAt(++i)])>>4;
            else{c=(b&15)<<12|B[s.charAt(++i)]<<6|B[s.charAt(++i)];b=B[s.charAt(++i)]}
            if(c)t+=String.fromCharCode(c)
        }
        return t
    })
};

EscapeUTF8=function(str){
    return str.replace(/[^*+.-9A-Z_a-z-]/g,function(s){
        var c=s.charCodeAt(0);
        return (c<16?"%0"+c.toString(16):c<128?"%"+c.toString(16):c<2048?"%"+(c>>6|192).toString(16)+"%"+(c&63|128).toString(16):"%"+(c>>12|224).toString(16)+"%"+(c>>6&63|128).toString(16)+"%"+(c&63|128).toString(16)).toUpperCase()
    })
};

UnescapeUTF8=function(str){
    return str.replace(/%(E(0%[AB]|[1-CEF]%[89AB]|D%[89])[0-9A-F]|C[2-9A-F]|D[0-9A-F])%[89AB][0-9A-F]|%[0-7][0-9A-F]/ig,function(s){
        var c=parseInt(s.substring(1),16);
        return String.fromCharCode(c<128?c:c<224?(c&31)<<6|parseInt(s.substring(4),16)&63:((c&15)<<6|parseInt(s.substring(4),16)&63)<<6|parseInt(s.substring(7),16)&63)
    })
};

EscapeUTF16LE=function(str){
    var H=function(c){
        return 41<c&&c<58&&44!=c||64<c&&c<91||95==c||96<c&&c<123?String.fromCharCode(c):(c<16?"%0":"%")+c.toString(16).toUpperCase()
    };
    return str.replace(/[^ ]| /g,function(s){
        var c=s.charCodeAt(0);return H(c&255)+H(c>>8)
    })
};

UnescapeUTF16LE=function(str){
    var u=String.fromCharCode,b=u(92,120,48,48,45,92,120,70,70);
    return str.replace(/^%FF%FE/i,"").replace(RegExp("%[0-9A-F]{2}%[0-9A-F]{2}|%[0-9A-F]{2}["+b+"]|["+b+"]%[0-9A-F]{2}|["+b+"]{2}","ig"),function(s){
        var l=s.length;
        return u(4<l?"0x"+s.substring(4,6)+s.substring(1,3):2<l?37==(l=s.charCodeAt(0))?parseInt(s.substring(1,3),16)|s.charCodeAt(3)<<8:l|parseInt(s.substring(2),16)<<8:s.charCodeAt(0)|s.charCodeAt(1)<<8)
    })
};

GetEscapeCodeType=function(str){
    if(/%u[0-9A-F]{4}/i.test(str))return "Unicode";
    if(/%([0-9A-DF][0-9A-F]%[8A]0%|E0%80|[0-7][0-9A-F]|C[01])%[8A]0|%00|%[7F]F/i.test(str))return "UTF16LE";
    if(/%E[0-9A-F]%[8A]0%[8A]0|%[CD][0-9A-F]%[8A]0/i.test(str))return "UTF8";
    if(/%F[DE]/i.test(str))return /%8[0-9A-D]|%9[0-9A-F]|%A0/i.test(str)?"UTF16LE":"EUCJP";
    if(/%1B/i.test(str))return /%[A-D][0-9A-F]/i.test(str)?"JIS8":"JIS7";
    var S=str.substring(0,6143).replace(/%[0-9A-F]{2}|[^ ]| /ig,function(s){
        return s.length<3?"40":s.substring(1)
    }),c,C,i=0,T;
    while(0<=(c=parseInt(S.substring(i,i+=2),16))&&i<4092)if(128<=c){
        if((C=parseInt(S.substring(i,i+2),16))<128)i+=2;
        else if(194<=c&&c<240&&C<192){
            if(c<224){T="UTF8";i+=2;continue}
            if(2==parseInt(S.charAt(i+2),16)>>2){T="UTF8";i+=4;continue}
        }
        if(142==c&&161<=C&&C<224){if(!T)T="EUCJP";if("EUCJP"==T)continue}
        if(c<161)return "SJIS";
        if(c<224&&!T)
            if((164==c&&C<244||165==c&&C<247)&&161<=C)i+=2;
            else T=224<=C?"EUCJP":"SJIS";
        else T="EUCJP"
    }
    return T?T:"EUCJP"
};

JCT11280=Function('var a="zKV33~jZ4zN=~ji36XazM93y!{~k2y!o~k0ZlW6zN?3Wz3W?{EKzK[33[`y|;-~j^YOTz$!~kNy|L1$353~jV3zKk3~k-4P4zK_2+~jY4y!xYHR~jlz$_~jk4z$e3X5He<0y!wy|X3[:~l|VU[F3VZ056Hy!nz/m1XD61+1XY1E1=1y|bzKiz!H034zKj~mEz#c5ZA3-3X$1~mBz$$3~lyz#,4YN5~mEz#{ZKZ3V%7Y}!J3X-YEX_J(3~mAz =V;kE0/y|F3y!}~m>z/U~mI~j_2+~mA~jp2;~m@~k32;~m>V}2u~mEX#2x~mBy+x2242(~mBy,;2242(~may->2&XkG2;~mIy-_2&NXd2;~mGz,{4<6:.:B*B:XC4>6:.>B*BBXSA+A:X]E&E<~r#z+625z s2+zN=`HXI@YMXIAXZYUM8X4K/:Q!Z&33 3YWX[~mB`{zKt4z (zV/z 3zRw2%Wd39]S11z$PAXH5Xb;ZQWU1ZgWP%3~o@{Dgl#gd}T){Uo{y5_d{e@}C(} WU9|cB{w}bzvV|)[} H|zT}d||0~{]Q|(l{|x{iv{dw}(5}[Z|kuZ }cq{{y|ij}.I{idbof%cu^d}Rj^y|-M{ESYGYfYsZslS`?ZdYO__gLYRZ&fvb4oKfhSf^d<Yeasc1f&a=hnYG{QY{D`Bsa|u,}Dl|_Q{C%xK|Aq}C>|c#ryW=}eY{L+`)][YF_Ub^h4}[X|?r|u_ex}TL@YR]j{SrXgo*|Gv|rK}B#mu{R1}hs|dP{C7|^Qt3|@P{YVV |8&}#D}ef{e/{Rl|>Hni}R1{Z#{D[}CQlQ||E}[s{SG_+i8eplY[=[|ec[$YXn#`hcm}YR|{Ci(_[ql|?8p3]-}^t{wy}4la&pc|3e{Rp{LqiJ],] `kc(]@chYnrM`O^,ZLYhZB]ywyfGY~aex!_Qww{a!|)*lHrM{N+n&YYj~Z b c#e_[hZSon|rOt`}hBXa^i{lh|<0||r{KJ{kni)|x,|0auY{D!^Sce{w;|@S|cA}Xn{C1h${E]Z-XgZ*XPbp]^_qbH^e[`YM|a||+=]!Lc}]vdBc=j-YSZD]YmyYLYKZ9Z>Xcczc2{Yh}9Fc#Z.l{}(D{G{{mRhC|L3b#|xK[Bepj#ut`H[,{E9Yr}1b{[e]{ZFk7[ZYbZ0XL]}Ye[(`d}c!|*y`Dg=b;gR]Hm=hJho}R-[n}9;{N![7k_{UbmN]rf#pTe[x8}!Qcs_rs[m`|>N}^V})7{^r|/E}),}HH{OYe2{Skx)e<_.cj.cjoMhc^d}0uYZd!^J_@g,[[[?{i@][|3S}Yl3|!1|eZ|5IYw|1D}e7|Cv{OHbnx-`wvb[6[4} =g+k:{C:}ed{S]|2M]-}WZ|/q{LF|dYu^}Gs^c{Z=}h>|/i|{W]:|ip{N:|zt|S<{DH[p_tvD{N<[8Axo{X4a.^o^X>Yfa59`#ZBYgY~_t^9`jZHZn`>G[oajZ;X,i)Z.^~YJe ZiZF^{][[#Zt^|]Fjx]&_5dddW]P0C[-]}]d|y {C_jUql] |OpaA[Z{lp|rz}:Mu#]_Yf6{Ep?f5`$[6^D][^u[$[6^.Z8]]ePc2U/=]K^_+^M{q*|9tYuZ,s(dS{i=|bNbB{uG}0jZOa:[-]dYtu3]:]<{DJ_SZIqr_`l=Yt`gkTnXb3d@kiq0a`Z{|!B|}e}Ww{Sp,^Z|0>_Z}36|]A|-t}lt{R6pi|v8hPu#{C>YOZHYmg/Z4nicK[}hF_Bg|YRZ7c|crkzYZY}_iXcZ.|)U|L5{R~qi^Uga@Y[xb}&qdbd6h5|Btw[}c<{Ds53[Y7]?Z<|e0{L[ZK]mXKZ#Z2^tavf0`PE[OSOaP`4gi`qjdYMgys/?[nc,}EEb,eL]g[n{E_b/vcvgb.{kcwi`~v%|0:|iK{Jh_vf5lb}KL|(oi=LrzhhY_^@`zgf[~g)[J_0fk_V{T)}I_{D&_/d9W/|MU[)f$xW}?$xr4<{Lb{y4}&u{XJ|cm{Iu{jQ}CMkD{CX|7A}G~{kt)nB|d5|<-}WJ}@||d@|Iy}Ts|iL|/^|no|0;}L6{Pm]7}$zf:|r2}?C_k{R(}-w|`G{Gy[g]bVje=_0|PT{^Y^yjtT[[[l!Ye_`ZN]@[n_)j3nEgMa]YtYpZy].d-Y_cjb~Y~[nc~sCi3|zg}B0}do{O^{|$`_|D{}U&|0+{J3|8*]iayx{a{xJ_9|,c{Ee]QXlYb]$[%YMc*]w[aafe]aVYi[fZEii[xq2YQZHg]Y~h#|Y:thre^@^|_F^CbTbG_1^qf7{L-`VFx Zr|@EZ;gkZ@slgko`[e}T:{Cu^pddZ_`yav^Ea+[#ZBbSbO`elQfLui}.F|txYcbQ`XehcGe~fc^RlV{D_0ZAej[l&jShxG[ipB_=u:eU}3e8[=j|{D(}dO{Do[BYUZ0/]AYE]ALYhZcYlYP/^-^{Yt_1_-;YT`P4BZG=IOZ&]H[e]YYd[9^F[1YdZxZ?Z{Z<]Ba2[5Yb[0Z4l?]d_;_)a?YGEYiYv`_XmZs4ZjY^Zb]6gqGaX^9Y}dXZr[g|]Y}K aFZp^k^F]M`^{O1Ys]ZCgCv4|E>}8eb7}l`{L5[Z_faQ|c2}Fj}hw^#|Ng|B||w2|Sh{v+[G}aB|MY}A{|8o}X~{E8paZ:]i^Njq]new)`-Z>haounWhN}c#{DfZ|fK]KqGZ=:u|fqoqcv}2ssm}.r{]{nIfV{JW)[K|,Z{Uxc|]l_KdCb%]cfobya3`p}G^|LZiSC]U|(X|kBlVg[kNo({O:g:|-N|qT}9?{MBiL}Sq{`P|3a|u.{Uaq:{_o|^S}jX{Fob0`;|#y_@[V[K|cw[<_ }KU|0F}d3|et{Q7{LuZttsmf^kYZ`Af`}$x}U`|Ww}d]| >}K,r&|XI|*e{C/a-bmr1fId4[;b>tQ_:]hk{b-pMge]gfpo.|(w[jgV{EC1Z,YhaY^q,_G[c_g[J0YX]`[h^hYK^_Yib,` {i6vf@YM^hdOKZZn(jgZ>bzSDc^Z%[[o9[2=/YHZ(_/Gu_`*|8z{DUZxYt^vuvZjhi^lc&gUd4|<UiA`z]$b/Z?l}YI^jaHxe|;F}l${sQ}5g}hA|e4}?o{ih}Uz{C)jPe4]H^J[Eg[|AMZMlc}:,{iz}#*|gc{Iq|/:|zK{l&}#u|myd{{M&v~nV};L|(g|I]ogddb0xsd7^V})$uQ{HzazsgxtsO^l}F>ZB]r|{7{j@cU^{{CbiYoHlng]f+nQ[bkTn/}<-d9q {KXadZYo+n|l[|lc}V2{[a{S4Zam~Za^`{HH{xx_SvF|ak=c^[v^7_rYT`ld@]:_ub%[$[m](Shu}G2{E.ZU_L_R{tz`vj(f?^}hswz}GdZ}{S:h`aD|?W|`dgG|if{a8|J1{N,}-Ao3{H#{mfsP|[ bzn+}_Q{MT{u4kHcj_q`eZj[8o0jy{p7}C|[}l){MuYY{|Ff!Ykn3{rT|m,^R|,R}$~Ykgx{P!]>iXh6[l[/}Jgcg{JYZ.^qYfYIZl[gZ#Xj[Pc7YyZD^+Yt;4;`e8YyZVbQ7YzZxXja.7SYl[s]2^/Ha$[6ZGYrb%XiYdf2]H]kZkZ*ZQ[ZYS^HZXcCc%Z|[(bVZ]]:OJQ_DZCg<[,]%Zaa [g{C00HY[c%[ChyZ,Z_`PbXa+eh`^&jPi0a[ggvhlekL]w{Yp^v}[e{~;k%a&k^|nR_z_Qng}[E}*Wq:{k^{FJZpXRhmh3^p>de^=_7`|ZbaAZtdhZ?n4ZL]u`9ZNc3g%[6b=e.ZVfC[ZZ^^^hD{E(9c(kyZ=bb|Sq{k`|vmr>izlH[u|e`}49}Y%}FT{[z{Rk}Bz{TCc/lMiAqkf(m$hDc;qooi[}^o:c^|Qm}a_{mrZ(pA`,}<2sY| adf_%|}`}Y5U;}/4|D>|$X{jw{C<|F.hK|*A{MRZ8Zsm?imZm_?brYWZrYx`yVZc3a@f?aK^ojEd {bN}/3ZH]/$YZhm^&j 9|(S|b]mF}UI{q&aM]LcrZ5^.|[j`T_V_Gak}9J[ ZCZD|^h{N9{~&[6Zd{}B}2O|cv]K}3s}Uy|l,fihW{EG`j_QOp~Z$F^zexS`dcISfhZBXP|.vn|_HYQ|)9|cr]<`&Z6]m_(ZhPcSg>`Z]5`~1`0Xcb4k1{O!bz|CN_T{LR|a/gFcD|j<{Z._[f)mPc:1`WtIaT1cgYkZOaVZOYFrEe[}T$}Ch}mk{K-^@]fH{Hdi`c*Z&|Kt{if[C{Q;{xYB`dYIX:ZB[}]*[{{p9|4GYRh2ao{DS|V+[zd$`F[ZXKadb*A] Ys]Maif~a/Z2bmclb8{Jro_rz|x9cHojbZ{GzZx_)]:{wAayeDlx}<=`g{H1{l#}9i|)=|lP{Qq}.({La|!Y{i2EZfp=c*}Cc{EDvVB|;g}2t{W4av^Bn=]ri,|y?|3+}T*ckZ*{Ffr5e%|sB{lx^0]eZb]9[SgAjS_D|uHZx]dive[c.YPkcq/}db{EQh&hQ|eg}G!ljil|BO]X{Qr_GkGl~YiYWu=c3eb}29v3|D|}4i||.{Mv})V{SP1{FX}CZW6{cm|vO{pS|e#}A~|1i}81|Mw}es|5[}3w{C`h9aL]o{}p[G`>i%a1Z@`Ln2bD[$_h`}ZOjhdTrH{[j_:k~kv[Sdu]CtL}41{I |[[{]Zp$]XjxjHt_eThoa#h>sSt8|gK|TVi[Y{t=}Bs|b7Zpr%{gt|Yo{CS[/{iteva|cf^hgn}($_c^wmb^Wm+|55jrbF|{9^ q6{C&c+ZKdJkq_xOYqZYSYXYl`8]-cxZAq/b%b*_Vsa[/Ybjac/OaGZ4fza|a)gY{P?| I|Y |,pi1n7}9bm9ad|=d{aV|2@[(}B`d&|Uz}B}{`q|/H|!JkM{FU|CB|.{}Az}#P|lk}K{|2rk7{^8^?`/|k>|Ka{Sq}Gz}io{DxZh[yK_#}9<{TRdgc]`~Z>JYmYJ]|`!ZKZ]gUcx|^E[rZCd`f9oQ[NcD_$ZlZ;Zr}mX|=!|$6ZPZYtIo%fj}CpcN|B,{VDw~gb}@hZg`Q{LcmA[(bo`<|@$|o1|Ss}9Z_}tC|G`{F/|9nd}i=}V-{L8aaeST]daRbujh^xlpq8|}zs4bj[S`J|]?G{P#{rD{]I`OlH{Hm]VYuSYUbRc*6[j`8]pZ[bt_/^Jc*[<Z?YE|Xb|?_Z^Vcas]h{t9|Uwd)_(=0^6Zb{Nc} E[qZAeX[a]P^|_J>e8`W^j_Y}R{{Jp__]Ee#e:iWb9q_wKbujrbR}CY`,{mJ}gz{Q^{t~N|? gSga`V_||:#mi}3t|/I`X{N*|ct|2g{km}gi|{={jC}F;|E}{ZZjYf*frmu}8Tdroi{T[|+~}HG{cJ}DM{Lp{Ctd&}$hi3|FZ| m}Kr|38}^c|m_|Tr{Qv|36}?Up>|;S{DV{k_as}BK{P}}9p|t`jR{sAm4{D=b4pWa[}Xi{EjwEkI}3S|E?u=X0{jf} S|NM|JC{qo^3cm]-|JUx/{Cj{s>{Crt[UXuv|D~|j|d{YXZR}Aq}0r}(_{pJfi_z}0b|-vi)Z mFe,{f4|q`b{}^Z{HM{rbeHZ|^x_o|XM|L%|uFXm}@C_{{Hhp%a7|0p[Xp+^K}9U{bP}: tT}B|}+$|b2|[^|~h{FAby[`{}xgygrt~h1[li`c4vz|,7p~b(|mviN}^pg[{N/|g3|^0c,gE|f%|7N{q[|tc|TKA{LU}I@|AZp(}G-sz{F |qZ{}F|f-}RGn6{Z]_5})B}UJ{FFb2]4ZI@v=k,]t_Dg5Bj]Z-]L]vrpdvdGlk|gF}G]|IW}Y0[G| /bo|Te^,_B}#n^^{QHYI[?hxg{[`]D^IYRYTb&kJ[cri[g_9]Ud~^_]<p@_e_XdNm-^/|5)|h_{J;{kacVopf!q;asqd}n)|.m|bf{QW|U)}b+{tL|w``N|to{t ZO|T]jF}CB|0Q{e5Zw|k |We}5:{HO{tPwf_uajjBfX}-V_C_{{r~gg|Ude;s+}KNXH}! `K}eW{Upwbk%ogaW}9EYN}YY|&v|SL{C3[5s.]Y]I]u{M6{pYZ`^,`ZbCYR[1mNg>rsk0Ym[jrE]RYiZTr*YJ{Ge|%-lf|y(`=[t}E6{k!|3)}Zk} ][G{E~cF{u3U.rJ|a9p#o#ZE|?|{sYc#vv{E=|LC}cu{N8`/`3`9rt[4|He{cq|iSYxY`}V |(Q|t4{C?]k_Vlvk)BZ^r<{CL}#h}R+[<|i=}X|{KAo]|W<`K{NW|Zx}#;|fe{IMr<|K~tJ_x}AyLZ?{GvbLnRgN}X&{H7|x~}Jm{]-| GpNu0}.ok>|c4{PYisrDZ|fwh9|hfo@{H~XSbO]Odv]%`N]b1Y]]|eIZ}_-ZA]aj,>eFn+j[aQ_+]h[J_m_g]%_wf.`%k1e#Z?{CvYu_B^|gk`Xfh^M3`afGZ-Z|[m{L}|k3cp[it ^>YUi~d>{T*}YJ{Q5{Jxa$hg|%4`}|LAgvb }G}{P=|<;Ux{_skR{cV|-*|s-{Mp|XP|$G|_J}c6cM{_=_D|*9^$ec{V;|4S{qO|w_|.7}d0|/D}e}|0G{Dq]Kdp{}dfDi>}B%{Gd|nl}lf{C-{y}|ANZr}#={T~|-(}c&{pI|ft{lsVP}){|@u}!W|bcmB{d?|iW|:dxj{PSkO|Hl]Li:}VYk@|2={fnWt{M3`cZ6|)}|Xj}BYa?vo{e4|L7|B7{L7|1W|lvYO}W8nJ|$Vih|{T{d*_1|:-n2dblk``fT{Ky|-%}m!|Xy|-a{Pz}[l{kFjz|iH}9N{WE{x,|jz}R {P|{D)c=nX|Kq|si}Ge{sh|[X{RF{t`|jsr*fYf,rK|/9}$}}Nf{y!1|<Std}4Wez{W${Fd_/^O[ooqaw_z[L`Nbv[;l7V[ii3_PeM}.h^viqYjZ*j1}+3{bt{DR[;UG}3Og,rS{JO{qw{d<_zbAh<R[1_r`iZTbv^^a}c{iEgQZ<exZFg.^Rb+`Uj{a+{z<[~r!]`[[|rZYR|?F|qppp]L|-d|}K}YZUM|=Y|ktm*}F]{D;g{uI|7kg^}%?Z%ca{N[_<q4xC]i|PqZC]n}.bDrnh0Wq{tr|OMn6tM|!6|T`{O`|>!]ji+]_bTeU}Tq|ds}n|{Gm{z,f)}&s{DPYJ`%{CGd5v4tvb*hUh~bf]z`jajiFqAii]bfy^U{Or|m+{I)cS|.9k:e3`^|xN}@Dnlis`B|Qo{`W|>||kA}Y}{ERYuYx`%[exd`]|OyiHtb}HofUYbFo![5|+]gD{NIZR|Go}.T{rh^4]S|C9_}xO^i`vfQ}C)bK{TL}cQ|79iu}9a];sj{P.o!f[Y]pM``Jda^Wc9ZarteBZClxtM{LW}l9|a.mU}KX}4@{I+f1}37|8u}9c|v${xGlz}jP{Dd1}e:}31}%3X$|22i<v+r@~mf{sN{C67G97855F4YL5}8f{DT|xy{sO{DXB334@55J1)4.G9A#JDYtXTYM4, YQD9;XbXm9SX]IB^4UN=Xn<5(;(F3YW@XkH-X_VM[DYM:5XP!T&Y`6|,^{IS-*D.H>:LXjYQ0I3XhAF:9:(==.F*3F1189K/7163D,:@|e2{LS36D4hq{Lw/84443@4.933:0307::6D7}&l{Mx657;89;,K5678H&93D(H<&<>0B90X^I;}Ag1{P%3A+>><975}[S{PZE453?4|T2{Q+5187;>447:81{C=hL6{Me^:=7ii{R=.=F<81;48?|h8}Uh{SE|,VxL{ST,7?9Y_5Xk3A#:$%YSYdXeKXOD8+TXh7(@>(YdXYHXl9J6X_5IXaL0N?3YK7Xh!1?XgYz9YEXhXaYPXhC3X`-YLY_XfVf[EGXZ5L8BXL9YHX]SYTXjLXdJ: YcXbQXg1PX]Yx4|Jr{Ys4.8YU+XIY`0N,<H%-H;:0@,74/:8546I=9177154870UC]d<C3HXl7ALYzXFXWP<<?E!88E5@03YYXJ?YJ@6YxX-YdXhYG|9o{`iXjY_>YVXe>AYFX[/(I@0841?):-B=14337:8=|14{c&93788|di{cW-0>0<097/A;N{FqYpugAFT%X/Yo3Yn,#=XlCYHYNX[Xk3YN:YRT4?)-YH%A5XlYF3C1=NWyY}>:74-C673<69545v {iT85YED=64=.F4..9878/D4378?48B3:7:7/1VX[f4{D,{l<5E75{dAbRB-8-@+;DBF/$ZfW8S<4YhXA.(5@*11YV8./S95C/0R-A4AXQYI7?68167B95HA1*<M3?1/@;/=54XbYP36}lc{qzSS38:19?,/39193574/66878Yw1X-87E6=;964X`T734:>86>1/=0;(I-1::7ALYGXhF+Xk[@W%TYbX7)KXdYEXi,H-XhYMRXfYK?XgXj.9HX_SX]YL1XmYJ>Y}WwIXiI-3-GXcYyXUYJ$X`Vs[7;XnYEZ;XF! 3;%8;PXX(N3Y[)Xi1YE&/ :;74YQ6X`33C;-(>Xm0(TYF/!YGXg8 9L5P01YPXO-5%C|qd{{/K/E6,=0144:361:955;6443@?B7*7:F89&F35YaX-CYf,XiFYRXE_e{}sF 0*7XRYPYfXa5YXXY8Xf8Y~XmA[9VjYj*#YMXIYOXk,HHX40YxYMXU8OXe;YFXLYuPXP?EB[QV0CXfY{:9XV[FWE0D6X^YVP*$4%OXiYQ(|xp|%c3{}V`1>Y`XH00:8/M6XhQ1:;3414|TE|&o@1*=81G8<3}6<|(f6>>>5-5:8;093B^3U*+*^*UT30XgYU&7*O1953)5@E78--F7YF*B&0:%P68W9Zn5974J9::3}Vk|-,C)=)1AJ4+<3YGXfY[XQXmT1M-XcYTYZXCYZXEYXXMYN,17>XIG*SaS|/eYJXbI?XdNZ+WRYP<F:R PXf;0Xg`$|1GX9YdXjLYxWX!ZIXGYaXNYm6X9YMX?9EXmZ&XZ#XQ>YeXRXfAY[4 ;0X!Zz0XdN$XhYL XIY^XGNXUYS/1YFXhYk.TXn4DXjB{jg|4DEX]:XcZMW=A.+QYL<LKXc[vV$+&PX*Z3XMYIXUQ:ZvW< YSXFZ,XBYeXMM)?Xa XiZ4/EXcP3%}&-|6~:1(-+YT$@XIYRBC<}&,|7aJ6}bp|8)K1|Xg|8C}[T|8Q.89;-964I38361<=/;883651467<7:>?1:.}le|:Z=39;1Y^)?:J=?XfLXbXi=Q0YVYOXaXiLXmJXO5?.SFXiCYW}-;|=u&D-X`N0X^,YzYRXO(QX_YW9`I|>hZ:N&X)DQXP@YH#XmNXi$YWX^=!G6YbYdX>XjY|XlX^XdYkX>YnXUXPYF)FXT[EVTMYmYJXmYSXmNXi#GXmT3X8HOX[ZiXN]IU2>8YdX1YbX<YfWuZ8XSXcZU%0;1XnXkZ_WTG,XZYX5YSX Yp 05G?XcYW(IXg6K/XlYP4XnI @XnO1W4Zp-9C@%QDYX+OYeX9>--YSXkD.YR%Q/Yo YUX].Xi<HYEZ2WdCE6YMXa7F)=,D>-@9/8@5=?7164;35387?N<618=6>7D+C50<6B03J0{Hj|N9$D,9I-,.KB3}m |NzE0::/81YqXjMXl7YG; [.W=Z0X4XQY]:MXiR,XgM?9$9>:?E;YE77VS[Y564760391?14941:0=:8B:;/1DXjFA-564=0B3XlH1+D85:0Q!B#:-6&N/:9<-R3/7Xn<*3J4.H:+334B.=>30H.;3833/76464665755:/83H6633:=;.>5645}&E|Y)?1/YG-,93&N3AE@5 <L1-G/8A0D858/30>8<549=@B8] V0[uVQYlXeD(P#ID&7T&7;Xi0;7T-$YE)E=1:E1GR):--0YI7=E<}n9|aT6783A>D7&4YG7=391W;Zx<5+>F#J39}o/|cc;6=A050EQXg8A1-}D-|d^5548083563695D?-.YOXd37I$@LYLWeYlX<Yd+YR A$;3-4YQ-9XmA0!9/XLY_YT(=5XdDI>YJ5XP1ZAW{9>X_6R(XhYO65&J%DA)C-!B:97#A9;@?F;&;(9=11/=657/H,<8}bz|j^5446>.L+&Y^8Xb6?(CYOXb*YF(8X`FYR(XPYVXmPQ%&DD(XmZXW??YOXZXfCYJ79,O)XnYF7K0!QXmXi4IYFRXS,6<%-:YO(+:-3Q!1E1:W,Zo}Am|n~;3580534*?3Zc4=9334361693:30C<6/717:<1/;>59&:4}6!|rS36=1?75<8}[B|s809983579I.A.>84758=108564741H*9E{L{|u%YQ<%6XfH.YUXe4YL@,>N}Tv|ve*G0X)Z;/)3@A74(4P&A1X:YVH97;,754*A66:1 D739E3553545558E4?-?K17/770843XAYf838A7K%N!YW4.$T19Z`WJ*0XdYJXTYOXNZ 1XaN1A+I&Xi.Xk3Z3GB&5%WhZ1+5#Y[X<4YMXhQYoQXVXbYQ8XSYUX4YXBXWDMG0WxZA[8V+Z8X;D],Va$%YeX?FXfX[XeYf<X:Z[WsYz8X_Y]%XmQ(!7BXIZFX]&YE3F$(1XgYgYE& +[+W!<YMYFXc;+PXCYI9YrWxGXY9DY[!GXiI7::)OC;*$.>N*HA@{C|}&k=:<TB83X`3YL+G4XiK]i}(fYK<=5$.FYE%4*5*H*6XkCYL=*6Xi6!Yi1KXR4YHXbC8Xj,B9ZbWx/XbYON#5B}Ue}+QKXnF1&YV5XmYQ0!*3IXBYb71?1B75XmF;0B976;H/RXU:YZX;BG-NXj;XjI>A#D3B636N;,*%<D:0;YRXY973H5)-4FXOYf0:0;/7759774;7;:/855:543L43<?6=E,.A4:C=L)%4YV!1(YE/4YF+ F3%;S;&JC:%/?YEXJ4GXf/YS-EXEYW,9;E}X$}547EXiK=51-?71C%?57;5>463553Zg90;6447?<>4:9.7538XgN{|!}9K/E&3-:D+YE1)YE/3;37/:05}n<}:UX8Yj4Yt864@JYK..G=.(A Q3%6K>3(P3#AYE$-6H/456*C=.XHY[#S.<780191;057C)=6HXj?955B:K1 E>-B/9,;5.!L?:0>/.@//:;7833YZ56<4:YE=/:7Z_WGC%3I6>XkC*&NA16X=Yz2$X:Y^&J48<99k8}CyB-61<18K946YO4{|N}E)YIB9K0L>4=46<1K0+R;6-=1883:478;4,S+3YJX`GJXh.Yp+Xm6MXcYpX(>7Yo,/:X=Z;Xi0YTYHXjYmXiXj;*;I-8S6N#XgY}.3XfYGO3C/$XjL$*NYX,1 6;YH&<XkK9C#I74.>}Hd`A748X[T450[n75<4439:18A107>|ET}Rf<1;14876/Yb983E<5.YNXd4149>,S=/4E/<306443G/06}0&}UkYSXFYF=44=-5095=88;63844,9E6644{PL}WA8:>)7+>763>>0/B3A545CCnT}Xm|dv}Xq1L/YNXk/H8;;.R63351YY747@15YE4J8;46;.38.>4A369.=-83,;Ye3?:3@YE.4-+N353;/;@(X[YYD>@/05-I*@.:551741Yf5>6A443<3535;.58/86=D4753442$635D1>0359NQ @73:3:>><Xn?;43C14 ?Y|X611YG1&<+,4<*,YLXl<1/AIXjF*N89A4Z576K1XbJ5YF.ZOWN.YGXO/YQ01:4G38Xl1;KI0YFXB=R<7;D/,/4>;$I,YGXm94@O35Yz66695385.>:6A#5}W7n^4336:4157597434433<3|XA}m`>=D>:4A.337370?-6Q96{`E|4A}C`|Qs{Mk|J+~r>|o,wHv>Vw}!c{H!|Gb|*Ca5}J||,U{t+{CN[!M65YXOY_*B,Y[Z9XaX[QYJYLXPYuZ%XcZ8LY[SYPYKZM<LMYG9OYqSQYM~[e{UJXmQYyZM_)>YjN1~[f3{aXFY|Yk:48YdH^NZ0|T){jVFYTZNFY^YTYN~[h{nPYMYn3I]`EYUYsYIZEYJ7Yw)YnXPQYH+Z.ZAZY]^Z1Y`YSZFZyGYHXLYG 8Yd#4~[i|+)YH9D?Y^F~Y7|-eYxZ^WHYdYfZQ~[j|3>~[k|3oYmYqY^XYYO=Z*4[]Z/OYLXhZ1YLZIXgYIHYEYK,<Y`YEXIGZI[3YOYcB4SZ!YHZ*&Y{Xi3~[l|JSY`Zz?Z,~[m|O=Yi>??XnYWXmYS617YVYIHZ(Z4[~L4/=~[n|Yu{P)|];YOHHZ}~[o33|a>~[r|aE]DH~[s|e$Zz~[t|kZFY~XhYXZB[`Y}~[u|{SZ&OYkYQYuZ2Zf8D~[v}% ~[w3},Q[X]+YGYeYPIS~[y}4aZ!YN^!6PZ*~[z}?E~[{3}CnZ=~[}}EdDZz/9A3(3S<,YR8.D=*XgYPYcXN3Z5 4)~[~}JW=$Yu.XX~] }KDX`PXdZ4XfYpTJLY[F5]X~[2Yp}U+DZJ::<446[m@~]#3}]1~]%}^LZwZQ5Z`/OT<Yh^ -~]&}jx[ ~m<z!%2+~ly4VY-~o>}p62yz!%2+Xf2+~ly4VY-zQ`z (=] 2z~o2",C={" ":0,"!":1},c=34,i=2,p,s="",u=String.fromCharCode,t=u(12539);while(++c<127)C[u(c)]=c^39&&c^92?i++:0;i=0;while(0<=(c=C[a.charAt(i++)]))if(16==c)if((c=C[a.charAt(i++)])<87){if(86==c)c=1879;while(c--)s+=u(++p)}else s+=s.substr(8272,360);else if(c<86)s+=u(p+=c<51?c-16:(c-55)*92+C[a.charAt(i++)]);else if((c=((c-86)*92+C[a.charAt(i++)])*92+C[a.charAt(i++)])<49152)s+=u(p=c<40960?c:c|57344);else{c&=511;while(c--)s+=t;p=12539}return s')();

JCT8836=JCT11280.substring(0,8836);

JCT11280=Function('var a="zKV33~jZ4zN=~ji36XazM93y!{~k2y!o~k0ZlW6zN?3Wz3W?{EKzK[33[`y|;-~j^YOTz$!~kNy|L1$353~jV3zKk3~k-4P4zK_2+~jY4y!xYHR~jlz$_~jk4z$e3X5He<0y!wy|X3[:~l|VU[F3VZ056Hy!nz/m1XD61+1XY1E1=1y|bzKiz!H034zKj~mEz#c5ZA3-3X$1~mBz$$3~lyz#,4YN5~mEz#{ZKZ3V%7Y}!J3X-YEX_J(3~mAz =V;kE0/y|F3y!}~m>z/U~mI~j_2+~mA~jp2;~m@~k32;~m>V}2u~mEX#2x~mBy+x2242(~mBy,;2242(~may->2&XkG2;~mIy-_2&NXd2;~mGz,{4<6:.:B*B:XC4>6:.>B*BBXSA+A:X]E&E<~r#z+625z s2+zN=`HXI@YMXIAXZYUM8X4K/:Q!Z&33 3YWX[~mB`{zKt4z (zV/z 3zRw2%Wd39]S11z$PAXH5Xb;ZQWU1ZgWP%3~o@{Dgl#gd}T){Uo{y5_d{e@}C(} WU9|cB{w}bzvV|)[} H|zT}d||0~{]Q|(l{|x{iv{dw}(5}[Z|kuZ }cq{{y|ij}.I{idbof%cu^d}Rj^y|-M{ESYGYfYsZslS`?ZdYO__gLYRZ&fvb4oKfhSf^d<Yeasc1f&a=hnYG{QY{D`Bsa|u,}Dl|_Q{C%xK|Aq}C>|c#ryW=}eY{L+`)][YF_Ub^h4}[X|?r|u_ex}TL@YR]j{SrXgo*|Gv|rK}B#mu{R1}hs|dP{C7|^Qt3|@P{YVV |8&}#D}ef{e/{Rl|>Hni}R1{Z#{D[}CQlQ||E}[s{SG_+i8eplY[=[|ec[$YXn#`hcm}YR|{Ci(_[ql|?8p3]-}^t{wy}4la&pc|3e{Rp{LqiJ],] `kc(]@chYnrM`O^,ZLYhZB]ywyfGY~aex!_Qww{a!|)*lHrM{N+n&YYj~Z b c#e_[hZSon|rOt`}hBXa^i{lh|<0||r{KJ{kni)|x,|0auY{D!^Sce{w;|@S|cA}Xn{C1h${E]Z-XgZ*XPbp]^_qbH^e[`YM|a||+=]!Lc}]vdBc=j-YSZD]YmyYLYKZ9Z>Xcczc2{Yh}9Fc#Z.l{}(D{G{{mRhC|L3b#|xK[Bepj#ut`H[,{E9Yr}1b{[e]{ZFk7[ZYbZ0XL]}Ye[(`d}c!|*y`Dg=b;gR]Hm=hJho}R-[n}9;{N![7k_{UbmN]rf#pTe[x8}!Qcs_rs[m`|>N}^V})7{^r|/E}),}HH{OYe2{Skx)e<_.cj.cjoMhc^d}0uYZd!^J_@g,[[[?{i@][|3S}Yl3|!1|eZ|5IYw|1D}e7|Cv{OHbnx-`wvb[6[4} =g+k:{C:}ed{S]|2M]-}WZ|/q{LF|dYu^}Gs^c{Z=}h>|/i|{W]:|ip{N:|zt|S<{DH[p_tvD{N<[8Axo{X4a.^o^X>Yfa59`#ZBYgY~_t^9`jZHZn`>G[oajZ;X,i)Z.^~YJe ZiZF^{][[#Zt^|]Fjx]&_5dddW]P0C[-]}]d|y {C_jUql] |OpaA[Z{lp|rz}:Mu#]_Yf6{Ep?f5`$[6^D][^u[$[6^.Z8]]ePc2U/=]K^_+^M{q*|9tYuZ,s(dS{i=|bNbB{uG}0jZOa:[-]dYtu3]:]<{DJ_SZIqr_`l=Yt`gkTnXb3d@kiq0a`Z{|!B|}e}Ww{Sp,^Z|0>_Z}36|]A|-t}lt{R6pi|v8hPu#{C>YOZHYmg/Z4nicK[}hF_Bg|YRZ7c|crkzYZY}_iXcZ.|)U|L5{R~qi^Uga@Y[xb}&qdbd6h5|Btw[}c<{Ds53[Y7]?Z<|e0{L[ZK]mXKZ#Z2^tavf0`PE[OSOaP`4gi`qjdYMgys/?[nc,}EEb,eL]g[n{E_b/vcvgb.{kcwi`~v%|0:|iK{Jh_vf5lb}KL|(oi=LrzhhY_^@`zgf[~g)[J_0fk_V{T)}I_{D&_/d9W/|MU[)f$xW}?$xr4<{Lb{y4}&u{XJ|cm{Iu{jQ}CMkD{CX|7A}G~{kt)nB|d5|<-}WJ}@||d@|Iy}Ts|iL|/^|no|0;}L6{Pm]7}$zf:|r2}?C_k{R(}-w|`G{Gy[g]bVje=_0|PT{^Y^yjtT[[[l!Ye_`ZN]@[n_)j3nEgMa]YtYpZy].d-Y_cjb~Y~[nc~sCi3|zg}B0}do{O^{|$`_|D{}U&|0+{J3|8*]iayx{a{xJ_9|,c{Ee]QXlYb]$[%YMc*]w[aafe]aVYi[fZEii[xq2YQZHg]Y~h#|Y:thre^@^|_F^CbTbG_1^qf7{L-`VFx Zr|@EZ;gkZ@slgko`[e}T:{Cu^pddZ_`yav^Ea+[#ZBbSbO`elQfLui}.F|txYcbQ`XehcGe~fc^RlV{D_0ZAej[l&jShxG[ipB_=u:eU}3e8[=j|{D(}dO{Do[BYUZ0/]AYE]ALYhZcYlYP/^-^{Yt_1_-;YT`P4BZG=IOZ&]H[e]YYd[9^F[1YdZxZ?Z{Z<]Ba2[5Yb[0Z4l?]d_;_)a?YGEYiYv`_XmZs4ZjY^Zb]6gqGaX^9Y}dXZr[g|]Y}K aFZp^k^F]M`^{O1Ys]ZCgCv4|E>}8eb7}l`{L5[Z_faQ|c2}Fj}hw^#|Ng|B||w2|Sh{v+[G}aB|MY}A{|8o}X~{E8paZ:]i^Njq]new)`-Z>haounWhN}c#{DfZ|fK]KqGZ=:u|fqoqcv}2ssm}.r{]{nIfV{JW)[K|,Z{Uxc|]l_KdCb%]cfobya3`p}G^|LZiSC]U|(X|kBlVg[kNo({O:g:|-N|qT}9?{MBiL}Sq{`P|3a|u.{Uaq:{_o|^S}jX{Fob0`;|#y_@[V[K|cw[<_ }KU|0F}d3|et{Q7{LuZttsmf^kYZ`Af`}$x}U`|Ww}d]| >}K,r&|XI|*e{C/a-bmr1fId4[;b>tQ_:]hk{b-pMge]gfpo.|(w[jgV{EC1Z,YhaY^q,_G[c_g[J0YX]`[h^hYK^_Yib,` {i6vf@YM^hdOKZZn(jgZ>bzSDc^Z%[[o9[2=/YHZ(_/Gu_`*|8z{DUZxYt^vuvZjhi^lc&gUd4|<UiA`z]$b/Z?l}YI^jaHxe|;F}l${sQ}5g}hA|e4}?o{ih}Uz{C)jPe4]H^J[Eg[|AMZMlc}:,{iz}#*|gc{Iq|/:|zK{l&}#u|myd{{M&v~nV};L|(g|I]ogddb0xsd7^V})$uQ{HzazsgxtsO^l}F>ZB]r|{7{j@cU^{{CbiYoHlng]f+nQ[bkTn/}<-d9q {KXadZYo+n|l[|lc}V2{[a{S4Zam~Za^`{HH{xx_SvF|ak=c^[v^7_rYT`ld@]:_ub%[$[m](Shu}G2{E.ZU_L_R{tz`vj(f?^}hswz}GdZ}{S:h`aD|?W|`dgG|if{a8|J1{N,}-Ao3{H#{mfsP|[ bzn+}_Q{MT{u4kHcj_q`eZj[8o0jy{p7}C|[}l){MuYY{|Ff!Ykn3{rT|m,^R|,R}$~Ykgx{P!]>iXh6[l[/}Jgcg{JYZ.^qYfYIZl[gZ#Xj[Pc7YyZD^+Yt;4;`e8YyZVbQ7YzZxXja.7SYl[s]2^/Ha$[6ZGYrb%XiYdf2]H]kZkZ*ZQ[ZYS^HZXcCc%Z|[(bVZ]]:OJQ_DZCg<[,]%Zaa [g{C00HY[c%[ChyZ,Z_`PbXa+eh`^&jPi0a[ggvhlekL]w{Yp^v}[e{~;k%a&k^|nR_z_Qng}[E}*Wq:{k^{FJZpXRhmh3^p>de^=_7`|ZbaAZtdhZ?n4ZL]u`9ZNc3g%[6b=e.ZVfC[ZZ^^^hD{E(9c(kyZ=bb|Sq{k`|vmr>izlH[u|e`}49}Y%}FT{[z{Rk}Bz{TCc/lMiAqkf(m$hDc;qooi[}^o:c^|Qm}a_{mrZ(pA`,}<2sY| adf_%|}`}Y5U;}/4|D>|$X{jw{C<|F.hK|*A{MRZ8Zsm?imZm_?brYWZrYx`yVZc3a@f?aK^ojEd {bN}/3ZH]/$YZhm^&j 9|(S|b]mF}UI{q&aM]LcrZ5^.|[j`T_V_Gak}9J[ ZCZD|^h{N9{~&[6Zd{}B}2O|cv]K}3s}Uy|l,fihW{EG`j_QOp~Z$F^zexS`dcISfhZBXP|.vn|_HYQ|)9|cr]<`&Z6]m_(ZhPcSg>`Z]5`~1`0Xcb4k1{O!bz|CN_T{LR|a/gFcD|j<{Z._[f)mPc:1`WtIaT1cgYkZOaVZOYFrEe[}T$}Ch}mk{K-^@]fH{Hdi`c*Z&|Kt{if[C{Q;{xYB`dYIX:ZB[}]*[{{p9|4GYRh2ao{DS|V+[zd$`F[ZXKadb*A] Ys]Maif~a/Z2bmclb8{Jro_rz|x9cHojbZ{GzZx_)]:{wAayeDlx}<=`g{H1{l#}9i|)=|lP{Qq}.({La|!Y{i2EZfp=c*}Cc{EDvVB|;g}2t{W4av^Bn=]ri,|y?|3+}T*ckZ*{Ffr5e%|sB{lx^0]eZb]9[SgAjS_D|uHZx]dive[c.YPkcq/}db{EQh&hQ|eg}G!ljil|BO]X{Qr_GkGl~YiYWu=c3eb}29v3|D|}4i||.{Mv})V{SP1{FX}CZW6{cm|vO{pS|e#}A~|1i}81|Mw}es|5[}3w{C`h9aL]o{}p[G`>i%a1Z@`Ln2bD[$_h`}ZOjhdTrH{[j_:k~kv[Sdu]CtL}41{I |[[{]Zp$]XjxjHt_eThoa#h>sSt8|gK|TVi[Y{t=}Bs|b7Zpr%{gt|Yo{CS[/{iteva|cf^hgn}($_c^wmb^Wm+|55jrbF|{9^ q6{C&c+ZKdJkq_xOYqZYSYXYl`8]-cxZAq/b%b*_Vsa[/Ybjac/OaGZ4fza|a)gY{P?| I|Y |,pi1n7}9bm9ad|=d{aV|2@[(}B`d&|Uz}B}{`q|/H|!JkM{FU|CB|.{}Az}#P|lk}K{|2rk7{^8^?`/|k>|Ka{Sq}Gz}io{DxZh[yK_#}9<{TRdgc]`~Z>JYmYJ]|`!ZKZ]gUcx|^E[rZCd`f9oQ[NcD_$ZlZ;Zr}mX|=!|$6ZPZYtIo%fj}CpcN|B,{VDw~gb}@hZg`Q{LcmA[(bo`<|@$|o1|Ss}9Z_}tC|G`{F/|9nd}i=}V-{L8aaeST]daRbujh^xlpq8|}zs4bj[S`J|]?G{P#{rD{]I`OlH{Hm]VYuSYUbRc*6[j`8]pZ[bt_/^Jc*[<Z?YE|Xb|?_Z^Vcas]h{t9|Uwd)_(=0^6Zb{Nc} E[qZAeX[a]P^|_J>e8`W^j_Y}R{{Jp__]Ee#e:iWb9q_wKbujrbR}CY`,{mJ}gz{Q^{t~N|? gSga`V_||:#mi}3t|/I`X{N*|ct|2g{km}gi|{={jC}F;|E}{ZZjYf*frmu}8Tdroi{T[|+~}HG{cJ}DM{Lp{Ctd&}$hi3|FZ| m}Kr|38}^c|m_|Tr{Qv|36}?Up>|;S{DV{k_as}BK{P}}9p|t`jR{sAm4{D=b4pWa[}Xi{EjwEkI}3S|E?u=X0{jf} S|NM|JC{qo^3cm]-|JUx/{Cj{s>{Crt[UXuv|D~|j|d{YXZR}Aq}0r}(_{pJfi_z}0b|-vi)Z mFe,{f4|q`b{}^Z{HM{rbeHZ|^x_o|XM|L%|uFXm}@C_{{Hhp%a7|0p[Xp+^K}9U{bP}: tT}B|}+$|b2|[^|~h{FAby[`{}xgygrt~h1[li`c4vz|,7p~b(|mviN}^pg[{N/|g3|^0c,gE|f%|7N{q[|tc|TKA{LU}I@|AZp(}G-sz{F |qZ{}F|f-}RGn6{Z]_5})B}UJ{FFb2]4ZI@v=k,]t_Dg5Bj]Z-]L]vrpdvdGlk|gF}G]|IW}Y0[G| /bo|Te^,_B}#n^^{QHYI[?hxg{[`]D^IYRYTb&kJ[cri[g_9]Ud~^_]<p@_e_XdNm-^/|5)|h_{J;{kacVopf!q;asqd}n)|.m|bf{QW|U)}b+{tL|w``N|to{t ZO|T]jF}CB|0Q{e5Zw|k |We}5:{HO{tPwf_uajjBfX}-V_C_{{r~gg|Ude;s+}KNXH}! `K}eW{Upwbk%ogaW}9EYN}YY|&v|SL{C3[5s.]Y]I]u{M6{pYZ`^,`ZbCYR[1mNg>rsk0Ym[jrE]RYiZTr*YJ{Ge|%-lf|y(`=[t}E6{k!|3)}Zk} ][G{E~cF{u3U.rJ|a9p#o#ZE|?|{sYc#vv{E=|LC}cu{N8`/`3`9rt[4|He{cq|iSYxY`}V |(Q|t4{C?]k_Vlvk)BZ^r<{CL}#h}R+[<|i=}X|{KAo]|W<`K{NW|Zx}#;|fe{IMr<|K~tJ_x}AyLZ?{GvbLnRgN}X&{H7|x~}Jm{]-| GpNu0}.ok>|c4{PYisrDZ|fwh9|hfo@{H~XSbO]Odv]%`N]b1Y]]|eIZ}_-ZA]aj,>eFn+j[aQ_+]h[J_m_g]%_wf.`%k1e#Z?{CvYu_B^|gk`Xfh^M3`afGZ-Z|[m{L}|k3cp[it ^>YUi~d>{T*}YJ{Q5{Jxa$hg|%4`}|LAgvb }G}{P=|<;Ux{_skR{cV|-*|s-{Mp|XP|$G|_J}c6cM{_=_D|*9^$ec{V;|4S{qO|w_|.7}d0|/D}e}|0G{Dq]Kdp{}dfDi>}B%{Gd|nl}lf{C-{y}|ANZr}#={T~|-(}c&{pI|ft{lsVP}){|@u}!W|bcmB{d?|iW|:dxj{PSkO|Hl]Li:}VYk@|2={fnWt{M3`cZ6|)}|Xj}BYa?vo{e4|L7|B7{L7|1W|lvYO}W8nJ|$Vih|{T{d*_1|:-n2dblk``fT{Ky|-%}m!|Xy|-a{Pz}[l{kFjz|iH}9N{WE{x,|jz}R {P|{D)c=nX|Kq|si}Ge{sh|[X{RF{t`|jsr*fYf,rK|/9}$}}Nf{y!1|<Std}4Wez{W${Fd_/^O[ooqaw_z[L`Nbv[;l7V[ii3_PeM}.h^viqYjZ*j1}+3{bt{DR[;UG}3Og,rS{JO{qw{d<_zbAh<R[1_r`iZTbv^^a}c{iEgQZ<exZFg.^Rb+`Uj{a+{z<[~r!]`[[|rZYR|?F|qppp]L|-d|}K}YZUM|=Y|ktm*}F]{D;g{uI|7kg^}%?Z%ca{N[_<q4xC]i|PqZC]n}.bDrnh0Wq{tr|OMn6tM|!6|T`{O`|>!]ji+]_bTeU}Tq|ds}n|{Gm{z,f)}&s{DPYJ`%{CGd5v4tvb*hUh~bf]z`jajiFqAii]bfy^U{Or|m+{I)cS|.9k:e3`^|xN}@Dnlis`B|Qo{`W|>||kA}Y}{ERYuYx`%[exd`]|OyiHtb}HofUYbFo![5|+]gD{NIZR|Go}.T{rh^4]S|C9_}xO^i`vfQ}C)bK{TL}cQ|79iu}9a];sj{P.o!f[Y]pM``Jda^Wc9ZarteBZClxtM{LW}l9|a.mU}KX}4@{I+f1}37|8u}9c|v${xGlz}jP{Dd1}e:}31}%3X$|22i<v+r@~mf{sN{C67G97855F4YL5}8f{DT|xy{sO{DXB334@55J1)4.G9A#JDYtXTYM4, YQD9;XbXm9SX]IB^4UN=Xn<5(;(F3YW@XkH-X_VM[DYM:5XP!T&Y`6|,^{IS-*D.H>:LXjYQ0I3XhAF:9:(==.F*3F1189K/7163D,:@|e2{LS36D4hq{Lw/84443@4.933:0307::6D7}&l{Mx657;89;,K5678H&93D(H<&<>0B90X^I;}Ag1{P%3A+>><975}[S{PZE453?4|T2{Q+5187;>447:81{C=hL6{Me^:=7ii{R=.=F<81;48?|h8}Uh{SE|,VxL{ST,7?9Y_5Xk3A#:$%YSYdXeKXOD8+TXh7(@>(YdXYHXl9J6X_5IXaL0N?3YK7Xh!1?XgYz9YEXhXaYPXhC3X`-YLY_XfVf[EGXZ5L8BXL9YHX]SYTXjLXdJ: YcXbQXg1PX]Yx4|Jr{Ys4.8YU+XIY`0N,<H%-H;:0@,74/:8546I=9177154870UC]d<C3HXl7ALYzXFXWP<<?E!88E5@03YYXJ?YJ@6YxX-YdXhYG|9o{`iXjY_>YVXe>AYFX[/(I@0841?):-B=14337:8=|14{c&93788|di{cW-0>0<097/A;N{FqYpugAFT%X/Yo3Yn,#=XlCYHYNX[Xk3YN:YRT4?)-YH%A5XlYF3C1=NWyY}>:74-C673<69545v {iT85YED=64=.F4..9878/D4378?48B3:7:7/1VX[f4{D,{l<5E75{dAbRB-8-@+;DBF/$ZfW8S<4YhXA.(5@*11YV8./S95C/0R-A4AXQYI7?68167B95HA1*<M3?1/@;/=54XbYP36}lc{qzSS38:19?,/39193574/66878Yw1X-87E6=;964X`T734:>86>1/=0;(I-1::7ALYGXhF+Xk[@W%TYbX7)KXdYEXi,H-XhYMRXfYK?XgXj.9HX_SX]YL1XmYJ>Y}WwIXiI-3-GXcYyXUYJ$X`Vs[7;XnYEZ;XF! 3;%8;PXX(N3Y[)Xi1YE&/ :;74YQ6X`33C;-(>Xm0(TYF/!YGXg8 9L5P01YPXO-5%C|qd{{/K/E6,=0144:361:955;6443@?B7*7:F89&F35YaX-CYf,XiFYRXE_e{}sF 0*7XRYPYfXa5YXXY8Xf8Y~XmA[9VjYj*#YMXIYOXk,HHX40YxYMXU8OXe;YFXLYuPXP?EB[QV0CXfY{:9XV[FWE0D6X^YVP*$4%OXiYQ(|xp|%c3{}V`1>Y`XH00:8/M6XhQ1:;3414|TE|&o@1*=81G8<3}6<|(f6>>>5-5:8;093B^3U*+*^*UT30XgYU&7*O1953)5@E78--F7YF*B&0:%P68W9Zn5974J9::3}Vk|-,C)=)1AJ4+<3YGXfY[XQXmT1M-XcYTYZXCYZXEYXXMYN,17>XIG*SaS|/eYJXbI?XdNZ+WRYP<F:R PXf;0Xg`$|1GX9YdXjLYxWX!ZIXGYaXNYm6X9YMX?9EXmZ&XZ#XQ>YeXRXfAY[4 ;0X!Zz0XdN$XhYL XIY^XGNXUYS/1YFXhYk.TXn4DXjB{jg|4DEX]:XcZMW=A.+QYL<LKXc[vV$+&PX*Z3XMYIXUQ:ZvW< YSXFZ,XBYeXMM)?Xa XiZ4/EXcP3%}&-|6~:1(-+YT$@XIYRBC<}&,|7aJ6}bp|8)K1|Xg|8C}[T|8Q.89;-964I38361<=/;883651467<7:>?1:.}le|:Z=39;1Y^)?:J=?XfLXbXi=Q0YVYOXaXiLXmJXO5?.SFXiCYW}-;|=u&D-X`N0X^,YzYRXO(QX_YW9`I|>hZ:N&X)DQXP@YH#XmNXi$YWX^=!G6YbYdX>XjY|XlX^XdYkX>YnXUXPYF)FXT[EVTMYmYJXmYSXmNXi#GXmT3X8HOX[ZiXN]IU2>8YdX1YbX<YfWuZ8XSXcZU%0;1XnXkZ_WTG,XZYX5YSX Yp 05G?XcYW(IXg6K/XlYP4XnI @XnO1W4Zp-9C@%QDYX+OYeX9>--YSXkD.YR%Q/Yo YUX].Xi<HYEZ2WdCE6YMXa7F)=,D>-@9/8@5=?7164;35387?N<618=6>7D+C50<6B03J0{Hj|N9$D,9I-,.KB3}m |NzE0::/81YqXjMXl7YG; [.W=Z0X4XQY]:MXiR,XgM?9$9>:?E;YE77VS[Y564760391?14941:0=:8B:;/1DXjFA-564=0B3XlH1+D85:0Q!B#:-6&N/:9<-R3/7Xn<*3J4.H:+334B.=>30H.;3833/76464665755:/83H6633:=;.>5645}&E|Y)?1/YG-,93&N3AE@5 <L1-G/8A0D858/30>8<549=@B8] V0[uVQYlXeD(P#ID&7T&7;Xi0;7T-$YE)E=1:E1GR):--0YI7=E<}n9|aT6783A>D7&4YG7=391W;Zx<5+>F#J39}o/|cc;6=A050EQXg8A1-}D-|d^5548083563695D?-.YOXd37I$@LYLWeYlX<Yd+YR A$;3-4YQ-9XmA0!9/XLY_YT(=5XdDI>YJ5XP1ZAW{9>X_6R(XhYO65&J%DA)C-!B:97#A9;@?F;&;(9=11/=657/H,<8}bz|j^5446>.L+&Y^8Xb6?(CYOXb*YF(8X`FYR(XPYVXmPQ%&DD(XmZXW??YOXZXfCYJ79,O)XnYF7K0!QXmXi4IYFRXS,6<%-:YO(+:-3Q!1E1:W,Zo}Am|n~;3580534*?3Zc4=9334361693:30C<6/717:<1/;>59&:4}6!|rS36=1?75<8}[B|s809983579I.A.>84758=108564741H*9E{L{|u%YQ<%6XfH.YUXe4YL@,>N}Tv|ve*G0X)Z;/)3@A74(4P&A1X:YVH97;,754*A66:1 D739E3553545558E4?-?K17/770843XAYf838A7K%N!YW4.$T19Z`WJ*0XdYJXTYOXNZ 1XaN1A+I&Xi.Xk3Z3GB&5%WhZ1+5#Y[X<4YMXhQYoQXVXbYQ8XSYUX4YXBXWDMG0WxZA[8V+Z8X;D],Va$%YeX?FXfX[XeYf<X:Z[WsYz8X_Y]%XmQ(!7BXIZFX]&YE3F$(1XgYgYE& +[+W!<YMYFXc;+PXCYI9YrWxGXY9DY[!GXiI7::)OC;*$.>N*HA@{C|}&k=:<TB83X`3YL+G4XiK]i}(fYK<=5$.FYE%4*5*H*6XkCYL=*6Xi6!Yi1KXR4YHXbC8Xj,B9ZbWx/XbYON#5B}Ue}+QKXnF1&YV5XmYQ0!*3IXBYb71?1B75XmF;0B976;H/RXU:YZX;BG-NXj;XjI>A#D3B636N;,*%<D:0;YRXY973H5)-4FXOYf0:0;/7759774;7;:/855:543L43<?6=E,.A4:C=L)%4YV!1(YE/4YF+ F3%;S;&JC:%/?YEXJ4GXf/YS-EXEYW,9;E}X$}547EXiK=51-?71C%?57;5>463553Zg90;6447?<>4:9.7538XgN{|!}9K/E&3-:D+YE1)YE/3;37/:05}n<}:UX8Yj4Yt864@JYK..G=.(A Q3%6K>3(P3#AYE$-6H/456*C=.XHY[#S.<780191;057C)=6HXj?955B:K1 E>-B/9,;5.!L?:0>/.@//:;7833YZ56<4:YE=/:7Z_WGC%3I6>XkC*&NA16X=Yz2$X:Y^&J48<99k8}CyB-61<18K946YO4{|N}E)YIB9K0L>4=46<1K0+R;6-=1883:478;4,S+3YJX`GJXh.Yp+Xm6MXcYpX(>7Yo,/:X=Z;Xi0YTYHXjYmXiXj;*;I-8S6N#XgY}.3XfYGO3C/$XjL$*NYX,1 6;YH&<XkK9C#I74.>}Hd`A748X[T450[n75<4439:18A107>|ET}Rf<1;14876/Yb983E<5.YNXd4149>,S=/4E/<306443G/06}0&}UkYSXFYF=44=-5095=88;63844,9E6644{PL}WA8:>)7+>763>>0/B3A545CCnT}Xm|dv}Xq1L/YNXk/H8;;.R63351YY747@15YE4J8;46;.38.>4A369.=-83,;Ye3?:3@YE.4-+N353;/;@(X[YYD>@/05-I*@.:551741Yf5>6A443<3535;.58/86=D4753442$635D1>0359NQ @73:3:>><Xn?;43C14 ?Y|X611YG1&<+,4<*,YLXl<1/AIXjF*N89A4Z576K1XbJ5YF.ZOWN.YGXO/YQ01:4G38Xl1;KI0YFXB=R<7;D/,/4>;$I,YGXm94@O35Yz66695385.>:6A#5}W7n^4336:4157597434433<3|XA}m`>=D>:4A.337370?-6Q96{`E|4A}C`|Qs{Mk|J+~r>|o,wHv>Vw}!c{H!|Gb|*Ca5}J||,U{t+{CN[!M65YXOY_*B,Y[Z9XaX[QYJYLXPYuZ%XcZ8LY[SYPYKZM<LMYG9OYqSQYM~[e{UJXmQYyZM_)>YjN1~[f3{aXFY|Yk:48YdH^NZ0|T){jVFYTZNFY^YTYN~[h{nPYMYn3I]`EYUYsYIZEYJ7Yw)YnXPQYH+Z.ZAZY]^Z1Y`YSZFZyGYHXLYG 8Yd#4~[i|+)YH9D?Y^F~Y7|-eYxZ^WHYdYfZQ~[j|3>~[k|3oYmYqY^XYYO=Z*4[]Z/OYLXhZ1YLZIXgYIHYEYK,<Y`YEXIGZI[3YOYcB4SZ!YHZ*&Y{Xi3~[l|JSY`Zz?Z,~[m|O=Yi>??XnYWXmYS617YVYIHZ(Z4[~L4/=~[n|Yu{P)|];YOHHZ}~[o33|a>~[r|aE]DH~[s|e$Zz~[t|kZFY~XhYXZB[`Y}~[u|{SZ&OYkYQYuZ2Zf8D~[v}% ~[w3},Q[X]+YGYeYPIS~[y}4aZ!YN^!6PZ*~[z}?E~[{3}CnZ=~[}}EdDZz/9A3(3S<,YR8.D=*XgYPYcXN3Z5 4)~[~}JW=$Yu.XX~] }KDX`PXdZ4XfYpTJLY[F5]X~[2Yp}U+DZJ::<446[m@~]#3}]1~]%}^LZwZQ5Z`/OT<Yh^ -~]&}jx[ ~m<z!%2+~ly4VY-~o>}p62yz!%2+Xf2+~ly4VY-zQ`z (=] 2z~o2",C={" ":0,"!":1},c=34,i=2,p,s=[],u=String.fromCharCode,t=u(12539);while(++c<127)C[u(c)]=c^39&&c^92?i++:0;i=0;while(0<=(c=C[a.charAt(i++)]))if(16==c)if((c=C[a.charAt(i++)])<87){if(86==c)c=1879;while(c--)s.push(u(++p))}else s.push(s.join("").substr(8272,360));else if(c<86)s.push(u(p+=c<51?c-16:(c-55)*92+C[a.charAt(i++)]));else if((c=((c-86)*92+C[a.charAt(i++)])*92+C[a.charAt(i++)])<49152)s.push(u(p=c<40960?c:c|57344));else{c&=511;while(c--)s.push(t);p=12539}return s.join("")')();
//----

// 以下、みみずさんによる成分解析のJavaScript実装をそのままです

function ftf(x, xx, yy){
    x += ""
    var buff = 0;
    var i = x.length;
    while(i){
        buff += strtocode(x.charAt(i - 1)) * Math.pow(xx, x.length - i);
        i--;
    }
    var ans = "";
    while(buff){
        ans = codetostr(buff % yy) + ans;
        buff = Math.floor(buff / yy);
    }
    if(ans == "")
        ans = "0";
    return ans;
}

function strtocode(x){
    var str = x.charCodeAt(0);
    if(49 <= str && str <= 57)
        return str - 48;
    else if(65 <= str && str <= 90)
        return str - 55;
    else if(97 <= str && str <= 122)
        return str - 87;
    else
        return null;
}

function codetostr(x){
    if(0 <= x && x <= 9)
        return String.fromCharCode(x + 48);
    else if(10 <= x && x <= 35)
        return String.fromCharCode(x + 87);
    else
        return null;
}

seibun_seed = 0;
function rand(){
    var result;
    seibun_seed *= 214013;
    seibun_seed %= 0x100000000;
    seibun_seed += 2531011;
    result = seibun_seed;
    result = result >> 0x10;
    result &= 0x7fff;
    return result;
}

function srand(x){
    seibun_seed = x;
}

function getshift_jis(x){
    var m;
    switch(x){
        case "!":m = "%21";break;
        case '"':m = "%22";break;
        case "#":m = "%23";break;
        case "$":m = "%24";break;
        case "%":m = "%25";break;
        case "&":m = "%26";break;
        case "'":m = "%27";break;
        case "(":m = "%28";break;
        case ")":m = "%29";break;
        case "*":m = "%2A";break;
        case "+":m = "%2B";break;
        case ",":m = "%2C";break;
        case "-":m = "%2D";break;
        case ".":m = "%2E";break;
        case "/":m = "%2F";break;
        case "0":m = "%30";break;
        case "1":m = "%31";break;
        case "2":m = "%32";break;
        case "3":m = "%33";break;
        case "4":m = "%34";break;
        case "5":m = "%35";break;
        case "6":m = "%36";break;
        case "7":m = "%37";break;
        case "8":m = "%38";break;
        case "9":m = "%39";break;
        case ":":m = "%3A";break;
        case ";":m = "%3B";break;
        case "<":m = "%3C";break;
        case "=":m = "%3D";break;
        case ">":m = "%3E";break;
        case "?":m = "%3F";break;
        case "@":m = "%40";break;
        case "A":m = "%41";break;
        case "B":m = "%42";break;
        case "C":m = "%43";break;
        case "D":m = "%44";break;
        case "E":m = "%45";break;
        case "F":m = "%46";break;
        case "G":m = "%47";break;
        case "H":m = "%48";break;
        case "I":m = "%49";break;
        case "J":m = "%4A";break;
        case "K":m = "%4B";break;
        case "L":m = "%4C";break;
        case "M":m = "%4D";break;
        case "N":m = "%4E";break;
        case "O":m = "%4F";break;
        case "P":m = "%50";break;
        case "Q":m = "%51";break;
        case "R":m = "%52";break;
        case "S":m = "%53";break;
        case "T":m = "%54";break;
        case "U":m = "%55";break;
        case "V":m = "%56";break;
        case "W":m = "%57";break;
        case "X":m = "%58";break;
        case "Y":m = "%59";break;
        case "Z":m = "%5A";break;
        case "[":m = "%5B";break;
        case "\\":m = "%5C";break;
        case "]":m = "%5D";break;
        case "^":m = "%5E";break;
        case "_":m = "%5F";break;
        case "`":m = "%60";break;
        case "a":m = "%61";break;
        case "b":m = "%62";break;
        case "c":m = "%63";break;
        case "d":m = "%64";break;
        case "e":m = "%65";break;
        case "f":m = "%66";break;
        case "g":m = "%67";break;
        case "h":m = "%68";break;
        case "i":m = "%69";break;
        case "j":m = "%6A";break;
        case "k":m = "%6B";break;
        case "l":m = "%6C";break;
        case "m":m = "%6D";break;
        case "n":m = "%6E";break;
        case "o":m = "%6F";break;
        case "p":m = "%70";break;
        case "q":m = "%71";break;
        case "r":m = "%72";break;
        case "s":m = "%73";break;
        case "t":m = "%74";break;
        case "u":m = "%75";break;
        case "v":m = "%76";break;
        case "w":m = "%77";break;
        case "x":m = "%78";break;
        case "y":m = "%79";break;
        case "z":m = "%7A";break;
        case "{":m = "%7B";break;
        case "|":m = "%7C";break;
        case "}":m = "%7D";break;
        case "~":m = "%7E";break;
        case " ":m = "%A0";break;
        case "｡":m = "%A1";break;
        case "｢":m = "%A2";break;
        case "｣":m = "%A3";break;
        case "､":m = "%A4";break;
        case "･":m = "%A5";break;
        case "ｦ":m = "%A6";break;
        case "ｧ":m = "%A7";break;
        case "ｨ":m = "%A8";break;
        case "ｩ":m = "%A9";break;
        case "ｪ":m = "%AA";break;
        case "ｫ":m = "%AB";break;
        case "ｬ":m = "%AC";break;
        case "ｭ":m = "%AD";break;
        case "ｮ":m = "%AE";break;
        case "ｯ":m = "%AF";break;
        case "ｰ":m = "%B0";break;
        case "ｱ":m = "%B1";break;
        case "ｲ":m = "%B2";break;
        case "ｳ":m = "%B3";break;
        case "ｴ":m = "%B4";break;
        case "ｵ":m = "%B5";break;
        case "ｶ":m = "%B6";break;
        case "ｷ":m = "%B7";break;
        case "ｸ":m = "%B8";break;
        case "ｹ":m = "%B9";break;
        case "ｺ":m = "%BA";break;
        case "ｻ":m = "%BB";break;
        case "ｼ":m = "%BC";break;
        case "ｽ":m = "%BD";break;
        case "ｾ":m = "%BE";break;
        case "ｿ":m = "%BF";break;
        case "ﾀ":m = "%C0";break;
        case "ﾁ":m = "%C1";break;
        case "ﾂ":m = "%C2";break;
        case "ﾃ":m = "%C3";break;
        case "ﾄ":m = "%C4";break;
        case "ﾅ":m = "%C5";break;
        case "ﾆ":m = "%C6";break;
        case "ﾇ":m = "%C7";break;
        case "ﾈ":m = "%C8";break;
        case "ﾉ":m = "%C9";break;
        case "ﾊ":m = "%CA";break;
        case "ﾋ":m = "%CB";break;
        case "ﾌ":m = "%CC";break;
        case "ﾍ":m = "%CD";break;
        case "ﾎ":m = "%CE";break;
        case "ﾏ":m = "%CF";break;
        case "ﾐ":m = "%D0";break;
        case "ﾑ":m = "%D1";break;
        case "ﾒ":m = "%D2";break;
        case "ﾓ":m = "%D3";break;
        case "ﾔ":m = "%D4";break;
        case "ﾕ":m = "%D5";break;
        case "ﾖ":m = "%D6";break;
        case "ﾗ":m = "%D7";break;
        case "ﾘ":m = "%D8";break;
        case "ﾙ":m = "%D9";break;
        case "ﾚ":m = "%DA";break;
        case "ﾛ":m = "%DB";break;
        case "ﾜ":m = "%DC";break;
        case "ﾝ":m = "%DD";break;
        case "ﾞ":m = "%DE";break;
        case "ﾟ":m = "%DF";break;
        default:m = EscapeSJIS(x);break;
    }
    return m;
}

function sort(elt){
    for (var i = (elt.length - 1) / 2 - 1; i > 0; i--) {
        var prev = 0;
        for (var j = 1; j <= i; j++) {
            if (!(elt[prev * 2 + 2] <= elt[j * 2 + 2])) {
                prev = j;
            }
        }
        elt = swap(elt, i, prev);
    }
    return elt;
}

function swap(elt, i, prev){
    var name = elt[i * 2 + 1];
    var p = elt[i * 2 + 2];
    elt[i * 2 + 1] = elt[prev * 2 + 1];
    elt[i * 2 + 2] = elt[prev * 2 + 2];
    elt[prev * 2 + 1] = name;
    elt[prev * 2 + 2] = p;
    return elt;
}

function seibunOf(str){
    var List = new Array("下心", "微妙さ", "優雅さ", "華麗さ", "かわいさ", "やさしさ", "やましさ", "やらしさ", "むなしさ", "ツンデレ", "厳しさ", "世の無常さ", "ハッタリ", "ビタミン", "努力", "気合", "根性", "砂糖", "食塩", "愛", "電波", "毒電波", "元気玉", "怨念", "大阪のおいしい水", "明太子", "勇気", "運", "電力", "小麦粉", "汗と涙(化合物)", "覚悟", "大人の都合", "見栄", "欲望", "嘘", "真空", "呪詛", "信念", "夢", "記憶", "鉄の意志", "カルシウム", "魔法", "希望", "不思議", "勢い", "度胸", "乙女心", "罠", "花崗岩", "宇宙の意思", "犠牲", "毒物", "鉛", "海水", "蛇の抜け殻", "波動", "純金", "情報", "知識", "知恵", "魂の炎", "媚び", "保存料", "着色料", "税金", "歌", "苦労", "柳の樹皮", "睡眠薬", "スライム", "アルコール", "時間", "果物", "玉露", "利益", "赤い何か", "白い何か", "鍛錬", "月の光", "回路", "野望", "陰謀", "雪の結晶", "株", "黒インク", "白インク", "カテキン", "祝福", "気の迷い", "マイナスイオン", "濃硫酸", "ミスリル", "お菓子", "言葉", "心の壁", "成功の鍵", "理論", "血");
    var s = "";
    var buf = new Array();
    var buff;
    var code = 0;
    for(var h = 0; h < str.length; h++)
        s += getshift_jis(str.substring(h, h + 1));
    s = s.split("%");
    var ii = 0;
    for(var i = 1; i < s.length; i++){
        buf[ii] = ftf(s[i].substring(0, 2), 16, 10) - 0;
        if(s[i].length == 3){
            ii++;
            buf[ii] = s[i].substring(2, 3).charCodeAt(0);
        }
        ii++
    }
    for(var j = 0; j < buf.length; j++)
        code += buf[j] << ((j & 3) << 3);
    code %= 0x100000000;
    srand(code);
    var p = 100;
    var data = new Array();
    data[0] = str;
    for(var k = 0; p; k += 2){
        var name; var par; var f;
        do{
            f = false;
            name = List[rand() % 100];
            for(var l = 0; l * 2 < data.length; l++)
                if(name == data[l * 2 + 1])
                    f = true;
        }while(f)
        par = rand() % p + 1;
        data[k + 1] = name; data[k + 2] = par;
        p -= par;
    }
    return sort(data);
}

// 以上引用

function seibun(prefix, target, noun)
{
    noun = trim(noun);
    var msg = noun + 'の成分: ';

    data = seibunOf(noun);
    for (var i = 1; i < data.length; i += 2) {
        if (i > 1)
            msg += ', '
        if (data[i + 1] == 100)
            msg += 'すべて' + data[i];
        else if (data[i + 1] == 50)
//            if (i == 1)
                msg += data[i] + '半分';
//            else
//                msg += data[i] + 'がもう半分';
        else
            msg += data[i] + data[i + 1] + '%';
    }
    textOut(target, msg);
}

//-- [ entry point ] -----------------------------------------------------------

function onTextPost(prefix, target, text)
{
    text = stripTextDecoration(text);

    var cfgCmd = new RegExp('^' + scriptName + '/([^ \t]+)[ \t]*(.*)[ \t]*');
    if (prefix.nick == myNick && text.match(cfgCmd)) {
        cmd = RegExp.$1;
        arg = RegExp.$2.replace(/\s+/g, ' ').split(' ');
        if (arg.length == 1 && arg[0] == '')
            arg.length = 0;
        scriptCfgCmd(prefix, target, cmd, arg)
    }

    if (!chList[target.toLowerCase()])
        return;

    if (text.match(/^\.seibun\s+(.*)/))
        seibun(prefix, target, RegExp.$1);
}

function event::onChannelText(prefix, channel, text)
{
    onTextPost(prefix, channel, text);
}

function event::onTalkText(prefix, targetNick, text)
{
    if (targetNick == myNick)
        onTextPost(prefix, prefix.nick, text);
    else
        onTextPost(prefix, targetNick, text);
}

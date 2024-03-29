/**
 * cl.js for LimeChat
 * Google Calcを通じて式を計算
 * 
 * スクリプトを動作させたいチャンネルで
 *  cl/start
 * と発言するか、あるいは
 *  cl/start #channel1 #channel2 ...
 * のように発言することで、スクリプトが有効になります。
 * 同様にexitでスクリプトを無効にできます。
 * ※スクリプト動作主の発言のみを受け付けます
 * 
 * あとは
 *  .cl <式>
 * で計算が可能です。
 * 
 * @author  gocha
 * @version 2012-01-01
 */

var scriptName = "cl";

//-- [ global / initialize ] ---------------------------------------------------

function event::onLoad()
{
    loadChList();
}

//-- [ customizable routines ] -------------------------------------------------

function privmsg(target, text)
{
    text = text.replace(/^\s+/, '').replace(/\s+$/, '');  // trim()
    var rawtext = 'PRIVMSG ' + target + ' :' + text;
    sendRaw(rawtext);
}

function textOut(target, text)
{
    //if (target == "#tasvideos") // FIXME: poor hack
        privmsg(target, text);
    //else
    //    send(target, text);
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

function cl(prefix, target, formula, lang)
{
    var xmlhttp = XMLHttpRequest();
    if (!xmlhttp) {
        errorOut(target, 'XMLHttpRequest() failed');
    }

    formula.replace(/\s+/g, ' ');
    formula = trim(formula);

    if (formula.match(/^0[Xbox][0-9A-Fa-f]+$/))
        formula += " in decimal";

    xmlhttp.onreadystatechange = function() {
        switch (xmlhttp.readyState) {
        case 1: // Open
        case 2: // Sent
        case 3: // Receiving
            break;
        case 4: // Loaded
            result = 'cl: something is technically wrong.';
            if (xmlhttp.status == 200) {
                calcResult = eval("(" + xmlhttp.responseText + ")");
                if (calcResult.error == "")
                {
                    var lhs = decodeCalcResult(calcResult.lhs);
                    var rhs = decodeCalcResult(calcResult.rhs);
                    result = "" + lhs + " = " + rhs;
                }
                else
                {
                    if (!formula.match(/[=＝]$/))
                    {
                        formula += ' =';
                        // send request again (to solve SEVEN-ELEVEn problem)
                        xmlhttp.setTimeouts(5*1000, 5*1000, 15*1000, 15*1000);
                        xmlhttp.open("GET", 'http://www.google.com/ig/calculator?hl=' + lang + '&ie=utf-8&oe=utf-8&q=' + encodeURIComponent(formula) , true);
                        xmlhttp.setRequestHeader('User-Agent', 'Mozilla/5.0 (compatible; limechat)');
                        xmlhttp.setRequestHeader('X-Request-Source', 'XMLHttpRequest');
                        xmlhttp.send('');
                        break;
                    }
                    else
                    {
                        if (lang == "ja")
                        {
                            result = "cl: エラー 計算できないよう(*っ○<) (" + calcResult.error + ")";
                        }
                        else
                        {
                            result = "cl: unable to calc the formula; error (" + calcResult.error + ")";
                        }
                    }
                }
            }
            if (result != '')
                textOut(target, result);
            break;
        }
    };
    xmlhttp.setTimeouts(5*1000, 5*1000, 15*1000, 15*1000);
    xmlhttp.open("GET", 'http://www.google.com/ig/calculator?hl=' + lang + '&ie=utf-8&oe=utf-8&q=' + encodeURIComponent(formula) , true);
    xmlhttp.setRequestHeader('User-Agent', 'Mozilla/5.0 (compatible; limechat)');
    xmlhttp.setRequestHeader('X-Request-Source', 'XMLHttpRequest');
    xmlhttp.send('');
}

function decodeCalcResult(result)
{
    result = decodeEntities(result);
    result = result.replace(/\xa0/g, ' ');
    result = result.replace(/<sup>(.*?)<\/sup>/g, '^$1');
    result = result.replace(/&times;/g, 'x');
    result = result.replace(/<.*?>/g, '');
    return result;
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

    if (text.match(/^\.cl\s+(.*)/)) {
        if (target == '#TASers') // FIXME: poor hack
            cl(prefix, target, RegExp.$1, 'ja');
        else
            cl(prefix, target, RegExp.$1, 'en');
    }
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

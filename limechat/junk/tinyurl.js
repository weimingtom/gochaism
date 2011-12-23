/**
 * tinyurl interface for LimeChat2.
 * tinyurlを含む発言に反応してURLをデコードする.
 * 
 * JScript慣れしない子がテキトーに書いた程度の代物.
 * ところどころ「個人的」な記述があるので注意です.
 * 細かいところ各所よりコード引っ張りです. 謝々.
 */

/**
 * PRIVMSGでチャンネルにメッセージを送信.
 */
function privmsg(channel, text)
{
    // TODO: 必要なら動作を改良する
    var rawtext = 'PRIVMSG ' + channel + ' :' + trim(text);
    sendRaw(rawtext);
}

/**
 * チャンネルメッセージを送信.
 */
function mysend(channel, text)
{
    if (name.match(/ustream/)) {
        privmsg(channel, text);
    }
    else {
        send(channel, text);
    }
}

//----

/**
 * Msxml2.ServerXMLHTTP を返す.
 */
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

/**
 * エンティティをデコード.
 */
var entity_list = {"ne":8800,"le":8804,"para":182,"xi":958,"darr":8595,"nu":957,"oacute":243,"Uacute":218,"omega":969,"prime":8242,"pound":163,"igrave":236,"thorn":254,"forall":8704,"emsp":8195,"lowast":8727,"brvbar":166,"alefsym":8501,"nbsp":160,"delta":948,"clubs":9827,"lArr":8656,"Omega":937,"quot":34,"Auml":196,"cedil":184,"and":8743,"plusmn":177,"ge":8805,"raquo":187,"uml":168,"equiv":8801,"laquo":171,"Epsilon":917,"rdquo":8221,"divide":247,"fnof":402,"chi":967,"Dagger":8225,"iacute":237,"rceil":8969,"sigma":963,"Oslash":216,"acute":180,"frac34":190,"upsih":978,"lrm":8206,"Scaron":352,"part":8706,"exist":8707,"nabla":8711,"image":8465,"prop":8733,"omicron":959,"zwj":8205,"gt":62,"aacute":225,"Yuml":376,"Yacute":221,"weierp":8472,"rsquo":8217,"otimes":8855,"kappa":954,"thetasym":977,"harr":8596,"Ouml":214,"Iota":921,"ograve":242,"sdot":8901,"copy":169,"oplus":8853,"acirc":226,"sup":8835,"zeta":950,"Iacute":205,"Oacute":211,"crarr":8629,"Nu":925,"bdquo":8222,"lsquo":8216,"Beta":914,"eacute":233,"egrave":232,"lceil":8968,"Kappa":922,"piv":982,"Ccedil":199,"ldquo":8220,"Xi":926,"cent":162,"uarr":8593,"hellip":8230,"Aacute":193,"ensp":8194,"sect":167,"Ugrave":217,"aelig":230,"ordf":170,"curren":164,"sbquo":8218,"macr":175,"Phi":934,"Eta":919,"rho":961,"Omicron":927,"sup2":178,"euro":8364,"aring":229,"Theta":920,"mdash":8212,"uuml":252,"otilde":245,"eta":951,"uacute":250,"rArr":8658,"nsub":8836,"agrave":224,"notin":8713,"Psi":936,"ndash":8211,"Ocirc":212,"sube":8838,"szlig":223,"micro":181,"not":172,"sup1":185,"middot":183,"iota":953,"ecirc":234,"lsaquo":8249,"thinsp":8201,"sum":8721,"ntilde":241,"scaron":353,"cap":8745,"atilde":227,"lang":9001,"isin":8712,"gamma":947,"Euml":203,"ang":8736,"upsilon":965,"Ntilde":209,"hearts":9829,"Tau":932,"Alpha":913,"spades":9824,"THORN":222,"dagger":8224,"int":8747,"lambda":955,"Eacute":201,"Uuml":220,"infin":8734,"Aring":197,"rlm":8207,"ugrave":249,"Egrave":200,"Acirc":194,"ETH":208,"oslash":248,"rsaquo":8250,"alpha":945,"Ograve":210,"Prime":8243,"mu":956,"ni":8715,"real":8476,"bull":8226,"beta":946,"icirc":238,"eth":240,"prod":8719,"larr":8592,"ordm":186,"perp":8869,"Gamma":915,"Pi":928,"reg":174,"ucirc":251,"psi":968,"tilde":732,"asymp":8776,"zwnj":8204,"Agrave":192,"Delta":916,"deg":176,"AElig":198,"times":215,"sim":8764,"Mu":924,"Otilde":213,"uArr":8657,"circ":710,"theta":952,"Rho":929,"sup3":179,"diams":9830,"tau":964,"Chi":935,"frac14":188,"oelig":339,"shy":173,"or":8744,"dArr":8659,"phi":966,"Lambda":923,"iuml":239,"rfloor":8971,"iexcl":161,"cong":8773,"ccedil":231,"Icirc":206,"frac12":189,"loz":9674,"rarr":8594,"cup":8746,"radic":8730,"frasl":8260,"euml":235,"OElig":338,"hArr":8660,"Atilde":195,"lt":60,"Upsilon":933,"there4":8756,"ouml":246,"oline":8254,"Ecirc":202,"yacute":253,"amp":38,"auml":228,"sigmaf":962,"permil":8240,"iquest":191,"empty":8709,"pi":960,"Ucirc":219,"supe":8839,"Igrave":204,"yen":165,"rang":9002,"trade":8482,"lfloor":8970,"minus":8722,"Zeta":918,"sub":8834,"epsilon":949,"Sigma":931,"yuml":255,"Iuml":207,"ocirc":244};
function decode_entities(str)
{
    return str.replace(/&#(\d+);?|&#x(\d+);?|&(AElig|Aacute|Acirc|Agrave|Alpha|Aring|Atilde|Auml|Beta|Ccedil|Chi|Dagger|Delta|ETH|Eacute|Ecirc|Egrave|Epsilon|Eta|Euml|Gamma|Iacute|Icirc|Igrave|Iota|Iuml|Kappa|Lambda|Mu|Ntilde|Nu|OElig|Oacute|Ocirc|Ograve|Omega|Omicron|Oslash|Otilde|Ouml|Phi|Pi|Prime|Psi|Rho|Scaron|Sigma|THORN|Tau|Theta|Uacute|Ucirc|Ugrave|Upsilon|Uuml|Xi|Yacute|Yuml|Zeta|aacute|acirc|acute|aelig|agrave|alefsym|alpha|amp|and|ang|aring|asymp|atilde|auml|bdquo|beta|brvbar|bull|cap|ccedil|cedil|cent|chi|circ|clubs|cong|copy|crarr|cup|curren|dArr|dagger|darr|deg|delta|diams|divide|eacute|ecirc|egrave|empty|emsp|ensp|epsilon|equiv|eta|eth|euml|euro|exist|fnof|forall|frac12|frac14|frac34|frasl|gamma|ge|gt|hArr|harr|hearts|hellip|iacute|icirc|iexcl|igrave|image|infin|int|iota|iquest|isin|iuml|kappa|lArr|lambda|lang|laquo|larr|lceil|ldquo|le|lfloor|lowast|loz|lrm|lsaquo|lsquo|lt|macr|mdash|micro|middot|minus|mu|nabla|nbsp|ndash|ne|ni|not|notin|nsub|ntilde|nu|oacute|ocirc|oelig|ograve|oline|omega|omicron|oplus|or|ordf|ordm|oslash|otilde|otimes|ouml|para|part|permil|perp|phi|pi|piv|plusmn|pound|prime|prod|prop|psi|quot|rArr|radic|rang|raquo|rarr|rceil|rdquo|real|reg|rfloor|rho|rlm|rsaquo|rsquo|sbquo|scaron|sdot|sect|shy|sigma|sigmaf|sim|spades|sub|sube|sum|sup|sup1|sup2|sup3|supe|szlig|tau|there4|theta|thetasym|thinsp|thorn|tilde|times|trade|uArr|uacute|uarr|ucirc|ugrave|uml|upsih|upsilon|uuml|weierp|xi|yacute|yen|yuml|zeta|zwj|zwnj);?/g,function(str, int, hex, ent){
        if(int){
            return String.fromCharCode(int);
        }else if(hex){
            return String.fromCharCode(parseInt("0x"+hex));
        }else if(entity_list[ent]){
            return String.fromCharCode(entity_list[ent]);
        }else{
            return "&" + ent;
        }
    });
}

//----

/**
 * スクリプトの動作を許可するかを返す.
 */
function isAllowed(channel)
{
    // I can't beat BisqBot
    if (channel.match(/^#nesvideos$/)) {
        return false;
    }
    return true;
}

var getTitle = new RegExp("<title[^<>]*>(.*?)</title>","i");
var getUrl = new RegExp('https?://[-_.!~*\'()a-zA-Z0-9;/?:@&=+$,%]+');//http://www.din.or.jp/~ohzaki/perl.htm#httpURL
var getCharsetHTML = new RegExp('<meta http-equiv="?content-type"? content="[^"]+charset=(.+?)"',"i");
var getCharsetXML = new RegExp('<\?xml[^>]+encoding="(.+?)"',"i");
/**
 * URLにアクセスして<title>タグがあれば取得.
 * 
 * original from:
 * gede99 @ http://pen2.com/url2title/
 */
function url2title(url)
{
    var xmlhttp = XMLHttpRequest();
    var result = null;

    if (xmlhttp) {
        var stream = new ActiveXObject("ADODB.Stream");
        xmlhttp.setTimeouts(5*1000, 5*1000, 15*1000, 15*1000);
        xmlhttp.open("GET", url , false);
        xmlhttp.setRequestHeader("Range","bytes=0-32768");
        xmlhttp.setRequestHeader("User-Agent","Mozilla/5.0 (compatible; url2title@limechat;)");
        xmlhttp.send("");

        if (xmlhttp.readyState == 4 && xmlhttp.status == 200 && xmlhttp.responseText.length) {
            stream.Open();
            stream.Type = 1;
            stream.Write( xmlhttp.responseBody );
            stream.Position = 0;
            stream.Type = 2;
            stream.Charset = getCharsetXML.exec(xmlhttp.responseText) ? RegExp.$1 : (getCharsetHTML.exec(xmlhttp.responseText) ? RegExp.$1 : "_autodetect");
            if(getTitle.exec( stream.ReadText() )){
                result = decode_entities(RegExp.$1).replace(/\s/g," ").substr(0,128);
            }
            stream.Close();
        }
    }
    return result;
}

var getTinyUrl = /tinyurl.com\/([A-Za-z0-9]+)/gi;
/**
 * チャンネルメッセージを処理.
 */
function event::onChannelText(prefix, channel, text)
{
    if (!isAllowed(channel))
        return;

    var urls = text.match(getTinyUrl);
    var num = (urls == null) ? 0 : urls.length;
    var max = 2;

    if (num < 1)
        return;
    else if (num > max) {
        mysend(channel, 'tinyurl: too much urls for me! I\'m lazy! ;)');
        return;
    }

    var ids = new Array(num);
    for (var key = 0; key < num; key++) {
        ids[key] = urls[key].replace(getTinyUrl, "$1");
    }

    var xmlhttp = XMLHttpRequest();
    if (xmlhttp) {
        for (var key = 0; key < num; key++) {
            urls[key] = null;

            // レスポンス等を気にするなら非同期になるよう書き直すといいかも
            xmlhttp.setTimeouts(5*1000, 5*1000, 15*1000, 15*1000);
            xmlhttp.open("GET", 'http://remysharp.com/tinyurlapi?url=tinyurl.com/' + encodeURIComponent(ids[key]), false);
            xmlhttp.send("");

            if (xmlhttp.readyState == 4 && xmlhttp.status == 200 && xmlhttp.responseText.length) {
                res = xmlhttp.responseText;
                if (res.match(/tinyurlCallback\("(.*)"\);/)) {
                    var url = RegExp.$1;

                    if (url != '' && url != 'null')
                        urls[key] = url;
                }
            }
        }
    }
    else {
        for (var key = 0; key < num; key++) {
            urls[key] = null;
        }
    }

    msg = '';
    for (var key = 0; key < num; key++) {
        if (key > 0)
            msg += ' ';

        if (urls[key] == null)
            msg += ids[key] + ' (Unknown)';
        else {
            var title = url2title(urls[key]);

            if (title == null)
                msg += ids[key] + ' ' + urls[key] + '';
            else
                msg += ids[key] + ' ' + urls[key] + ' (' + title + ')';
        }
    }
    mysend(channel, msg);
}

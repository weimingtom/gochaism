function event::onChannelText(prefix, channel, text)
{
  if (text.match("^\./ini$"))
    test_ini();
}

function test_ini(){
  var testfile = "sample.ini";
  var parser = new INI(testfile);
  parser.dump();
  // parser.toJSON;
}
/* 
  modulesで返されるのは、ファイル先頭から順に登場したセクションごとの
  情報を格納したオブジェクトの配列。
  <modules>
  modules[0]: name="section1", key="value", ...
  modules[1]: name="section2", key="value", ...
 */
function INI(path){
  var re = {
    mod: /^\[([^\[\]]+)\]/, /* section header */
    emp: /^;/, /* comment out */
    key_pair: /([^;=\[\]]+)=([^;=\[\]]+)/
  }
  this.puts = function(str){
    log(str);
  }
  this.dump = function(){
    var output = "";
    for(var i=0,max=iniData.length; i<max; i++){
      output = output + iniData[i]["name"] + ' {\n';
      for(var j in iniData[i]){
        output = output + '  ' + j + ': ' + iniData[i][j] + ',\n';
      }
      output = output + '}\n';
    }
    this.puts(output);
  }
  this.toJSON = function(){
    this.puts("not implemented yet.");
    return null;
  }
  this.readfile = function(path){
    var file = openFile(path);
    var lines = [];
    if (file) {
      var line;
      while (line = file.readLine()) {
        lines[lines.length] = line;
      }
      file.close();
    }
    return lines;
  }
  this.parse = function(lines){
    var modules = new Array;
    var pos = -1;
    var modname = "";
    for(var i=0, max=lines.length; i<max; i++){
      var line = lines[i];
      if(line.match(re.emp) != null){
        continue;
      }
      if(line.match(re.mod) != null){
        if(modname == "" || modname != RegExp.$1){
          modname = RegExp.$1;
          pos++;
          modules[pos] = new Object;
          modules[pos]["name"] = modname;
        }
        continue;
      }
      if(line.match(re.key_pair) != null){
        var key = RegExp.$1;
        var valuestr = RegExp.$2;
        modules[pos][key] = valuestr;
      }
    }
    return modules;
  }
  var lines = this.readfile(path);
  var iniData = this.parse(lines);
}

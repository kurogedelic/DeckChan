#include "WebEditor.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include "DeckConfig.h"
#include "DataHub.h"

static WebServer server(80);
static const char INDEX_HTML[] PROGMEM=R"HTML(
<!doctype html><html><head><meta name=viewport content="width=device-width,initial-scale=1"><title>DeckChan</title><style>
:root{--fg:#ffad32;--bg:#090704}*{box-sizing:border-box}body{background:var(--bg);color:var(--fg);font:14px monospace;max-width:820px;margin:28px auto;padding:0 18px}h1{font-size:22px}fieldset{border:1px solid var(--fg);margin:12px 0;padding:14px;display:grid;gap:10px}label{display:grid;grid-template-columns:180px 1fr;gap:10px;align-items:center}input,select,button{font:inherit;background:var(--bg);color:var(--fg);border:1px solid var(--fg);padding:7px}button{cursor:pointer}.row{display:flex;gap:8px;flex-wrap:wrap}.dim{opacity:.6}@media(max-width:560px){label{grid-template-columns:1fr}}
</style></head><body><h1>DECKCHAN // EDITOR</h1><div id=net class=dim></div>
<fieldset><legend>DISPLAY</legend><label>Palette<select id=palette><option>amber</option><option>green</option><option>white</option><option>ice</option></select></label><label>Idle after (sec)<input id=idle type=number min=5></label><label>Refresh (sec)<input id=refresh type=number min=30></label><label>Active brightness<input id=activeBrightness type=range min=1 max=255></label><label>Idle brightness<input id=idleBrightness type=range min=1 max=255></label></fieldset>
<fieldset><legend>SOURCES</legend><label>Weather URL<input id=weatherUrl></label><label>Calendar URL<input id=calendarUrl></label><label>Homebridge URL<input id=homebridgeUrl></label><label>Homebridge token<input id=homebridgeToken type=password placeholder="unchanged if blank"></label></fieldset>
<fieldset><legend>NIGHT</legend><label>Enabled<input id=nightEnabled type=checkbox></label><label>Start hour<input id=nightStart type=number min=0 max=23></label><label>End hour<input id=nightEnd type=number min=0 max=23></label></fieldset>
<div class=row><button onclick=save()>SAVE</button><button onclick=refreshData()>REFRESH DATA</button></div><pre id=status></pre>
<script>
const ids=['palette','idle','refresh','weatherUrl','calendarUrl','homebridgeUrl','activeBrightness','idleBrightness','nightStart','nightEnd'];
async function load(){let c=await(await fetch('/api/config')).json();ids.forEach(k=>{if(c[k]!==undefined)document.getElementById(k).value=c[k]});nightEnabled.checked=!!c.nightEnabled;let s=await(await fetch('/api/status')).json();net.textContent=s.ip+' // '+s.rssi+' dBm'}
async function save(){let o={};ids.forEach(k=>o[k]=document.getElementById(k).type==='number'||document.getElementById(k).type==='range'?+document.getElementById(k).value:document.getElementById(k).value);o.nightEnabled=nightEnabled.checked;o.homebridgeToken=homebridgeToken.value;let r=await fetch('/api/config',{method:'POST',body:JSON.stringify(o)});status.textContent=r.ok?'SAVED':'ERROR'}
async function refreshData(){await fetch('/api/refresh',{method:'POST'});status.textContent='REFRESHED'}load();
</script></body></html>)HTML";

void WebEditor::begin(){
  MDNS.begin("deckchan"); MDNS.addService("http","tcp",80);
  server.on("/",[](){server.send_P(200,"text/html",INDEX_HTML);});
  server.on("/api/status",[](){server.send(200,"application/json","{\"ip\":\""+WiFi.localIP().toString()+"\",\"rssi\":"+String(WiFi.RSSI())+"}");});
  server.on("/api/config",HTTP_GET,[](){server.send(200,"application/json",deckConfigJson(false));});
  server.on("/api/config",HTTP_POST,[](){bool ok=updateDeckConfigJson(server.arg("plain"))&&saveDeckConfig();server.send(ok?200:400,"application/json",ok?"{\"ok\":true}":"{\"ok\":false}");});
  server.on("/api/refresh",HTTP_POST,[](){dataHub.refresh();server.send(200,"application/json","{\"ok\":true}");});
  server.on("/api/notify",HTTP_POST,[](){dataHub.notify(server.arg("plain"));server.send(200,"application/json","{\"ok\":true}");});
  server.on("/update",HTTP_POST,[](){server.send(Update.hasError()?500:200,"text/plain",Update.hasError()?"FAIL":"OK");if(!Update.hasError()){delay(300);ESP.restart();}},[](){HTTPUpload& u=server.upload();if(u.status==UPLOAD_FILE_START)Update.begin(UPDATE_SIZE_UNKNOWN);else if(u.status==UPLOAD_FILE_WRITE)Update.write(u.buf,u.currentSize);else if(u.status==UPLOAD_FILE_END)Update.end(true);});
  server.begin();
}
void WebEditor::loop(){server.handleClient();}

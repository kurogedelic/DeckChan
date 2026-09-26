#include "WebEditor.h"
#include <WiFi.h>
#include <WebServer.h>
#include "DeckConfig.h"

static WebServer server(80);

static const char INDEX_HTML[] PROGMEM = R"HTML(
<!doctype html><html><head><meta name=viewport content="width=device-width,initial-scale=1">
<title>DeckChan</title><style>
body{background:#090704;color:#ffad32;font:16px monospace;max-width:720px;margin:40px auto;padding:0 20px}
*{box-sizing:border-box}fieldset{border:1px solid #ffad32;margin:16px 0;padding:16px}
input,select,button{font:inherit;background:#090704;color:#ffad32;border:1px solid #ffad32;padding:8px}
button{cursor:pointer}h1{font-size:24px}.dim{opacity:.6}
</style></head><body>
<h1>DECKCHAN // CONFIG</h1><p class=dim>LOCAL CONSOLE</p>
<fieldset><legend>DISPLAY</legend>
<label>Palette <select id=palette><option>amber</option><option>green</option><option>white</option><option>ice</option></select></label>
</fieldset>
<fieldset><legend>IDLE</legend>
<label>Return after <input id=idle type=number min=5 value=20> sec</label>
</fieldset>
<button onclick=save()>SAVE</button>
<pre id=status></pre>
<script>
async function save(){
 const body={palette:palette.value,idle:+idle.value};
 const r=await fetch('/api/config',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify(body)});
 status.textContent=r.ok?'SAVED':'ERROR';
}
</script></body></html>
)HTML";

void WebEditor::begin() {
  server.on("/", [](){ server.send_P(200, "text/html", INDEX_HTML); });
  server.on("/api/status", [](){
    String json = "{\"name\":\"" + deckConfig.deviceName + "\",\"rssi\":" + String(WiFi.RSSI()) + "}";
    server.send(200, "application/json", json);
  });
  server.on("/api/config", HTTP_POST, [](){
    // JSON persistence is intentionally the next step; endpoint exists now
    // so the browser contract can stabilize first.
    server.send(200, "application/json", "{\"ok\":true}");
  });
  server.begin();
}

void WebEditor::loop() { server.handleClient(); }

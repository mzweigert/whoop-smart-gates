document.addEventListener("DOMContentLoaded", function () {

    let loader = document.getElementById('loader');

    function createTableHeader(order) {
        let $th = document.createElement('th');
        $th.setAttribute('scope', 'col');
        $th.classList.add('text-center');
        $th.appendChild(document.createTextNode('Gate order: ' + order));
        return $th;
    }

    function createTableData(ws, savedColors) {
        let $td = document.createElement('td'),
            $container = document.createElement('div');
        $container.style.display = 'flex';
        $container.style.justifyContent = 'center';
        new iro.ColorPicker($container, { color: savedColors })
            .on('input:end', function (color) {
                let ledStripState = color.rgb;
                ledStripState['order'] = savedColors['order'];
                let data = { 'eventType': 'CHANGE_COLOR', 'payload': ledStripState }
                loader.style.display = 'block';
                ws.send(JSON.stringify(data));
            });
        $td.appendChild($container);
        return $td;
    }

    function createGUIforMobile(ws, savedColors) {
        let $colorPickersContainer = document.getElementById('color-pickers-container');
        for (let savedColor of savedColors) {
            let $table = document.createElement('table'),
                $thead = document.createElement("thead"),
                $tbody = document.createElement("tbody"),
                $tr = document.createElement("tr");
            let $th = createTableHeader(savedColor['order']);
            $thead.appendChild($th);
            let $td = createTableData(ws, savedColor);
            $tr.appendChild($td);
            $tbody.appendChild($tr)
            $table.appendChild($thead);
            $table.appendChild($tbody);
            $table.classList.add('table');
            $colorPickersContainer.appendChild($table);
        }
    }

    function createGUIforDesktop(ws, savedColors) {
        let $table = document.createElement('table'),
            $thead = document.createElement("thead"),
            $tbody = document.createElement("tbody"),
            $tr = document.createElement("tr");
        for (let savedColor of savedColors) {
            let $th = createTableHeader(savedColor['order']);
            $thead.appendChild($th);
            let $td = createTableData(ws, savedColor);
            $tr.appendChild($td);
        }
        $tbody.appendChild($tr);
        $table.appendChild($thead);
        $table.appendChild($tbody);
        $table.classList.add('table');
        document.getElementById('color-pickers-container').appendChild($table);
    }

    function addStyleToToggleBtn(state) {
        let btn = document.getElementById('leds-enabled');
        btn.setAttribute("checked", state)
        btn.className = '';
        btn.classList.add('btn');
        btn.classList.add(state ? 'btn-success' : 'btn-danger');
        btn.innerText = state ? 'Enabled' : 'Disabled';
        return btn;
    }

    function initializeToggleBtn(ws, enabled) {
        let btn = addStyleToToggleBtn(enabled > 0);
        btn.addEventListener("click", function () {
            let checked = this.getAttribute("checked") == 'true';
            loader.style.display = 'block';
            ws.send(JSON.stringify({ eventType: "SWITCH_ON_OFF", payload: !checked }));
            addStyleToToggleBtn(!checked);
        });
    }

    function initialize(ws, state) {
        let enabled = state['enabled'];
        initializeToggleBtn(ws, enabled);
        if (/Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.userAgent)) {
            createGUIforMobile(ws, state['ledStrips']);
        } else {
            createGUIforDesktop(ws, state['ledStrips']);
        }
    }

    (function startSocket() {
        let ws = new WebSocket('ws://' + document.location.host + '/led-strips-ws', ['arduino']);
        ws.onopen = function (e) {
            console.log(e);
            ws.send(JSON.stringify({ eventType: "INITIALIZE" }))
        };
        ws.onclose = function (e) {
            console.log("Disconnected");
            document.location.reload();
        };
        ws.onerror = function (e) {
            console.log("ws error", e);
            console.log("Error");
            document.location.reload();
        };
        ws.onmessage = function (e) {
            data = JSON.parse(e.data);
            if (data.eventType == "INITIALIZE") {
                initialize(ws, data['payload'])
                loader.style.display = 'none';
            } else if (data.eventType == "CHANGE_COLOR") {
                loader.style.display = 'none';
            } else if (data.eventType == "SWITCH_ON_OFF") {
                loader.style.display = 'none';
            }
        };
        return ws;
    })();
});
document.addEventListener("DOMContentLoaded", function() { 
    let host = '';
    
    function httpRequest(url, method, callback = null, body = null) {
        let xmlHttp = new XMLHttpRequest();
        xmlHttp.onreadystatechange = function() { 
            if (xmlHttp.readyState == 4 && xmlHttp.status == 200 && callback) {
                let responseBody;
                try {
                    responseBody = JSON.parse(xmlHttp.response);
                } catch (e) {
                    responseBody = xmlHttp.response;
                }
                callback(responseBody);
            }
        }
        xmlHttp.open(method, url, true); // true for asynchronous 
        if(body) {
            let json = JSON.stringify(body);
            xmlHttp.setRequestHeader("Content-Type", "application/json");
            xmlHttp.send(json);
        } else xmlHttp.send();
    }

    function onColorChange(order, colorRGB) {
        let data = colorRGB;
        data['order'] = Number(order);
        httpRequest(host + '/changeColor', 'POST', null, data);
    }

    function createTableHeader(order) {
        let $th = document.createElement('th');
            $th.setAttribute('scope', 'col');
            $th.classList.add('text-center');
            $th.appendChild(document.createTextNode('Gate order: ' + order ));
        return $th;
    }
    function createTableData(order, savedColors) {
        let $td = document.createElement('td'), 
            $container = document.createElement('div'); 
            $container.style.display = 'flex' ;
            $container.style.justifyContent = 'center';
            let colors = savedColors.filter(obj => obj.order == order);
            new iro.ColorPicker($container, { colors: colors })
                .on('input:end', function (color) { onColorChange(order, color.rgb); });
            $td.appendChild($container);
        return $td;
    }

    function createGUIforMobile(orders, savedColors) {
        let $colorPickersContainer = document.getElementById('color-pickers-container');
        for (let order of orders) {
            let $table = document.createElement('table'), 
            $thead = document.createElement("thead"), 
            $tbody = document.createElement("tbody"),
            $tr = document.createElement("tr");
            let $th = createTableHeader(order);
            $thead.appendChild($th);
            let $td = createTableData(order, savedColors);
            $tr.appendChild($td);
            $tbody.appendChild($tr)
            $table.appendChild($thead);
            $table.appendChild($tbody);
            $table.classList.add('table');
            $colorPickersContainer.appendChild($table);
        }
    }

    function createGUIforDesktop(orders, savedColors) {
        let $table = document.createElement('table'), 
            $thead = document.createElement("thead"), 
            $tbody = document.createElement("tbody"),
            $tr    = document.createElement("tr");
        for (let order of orders) {
            let $th = createTableHeader(order);
            $thead.appendChild($th);
            let $td = createTableData(order, savedColors);
            $tr.appendChild($td);
        }
        $tbody.appendChild($tr);
        $table.appendChild($thead);
        $table.appendChild($tbody);
        $table.classList.add('table');
        document.getElementById('color-pickers-container').appendChild($table);
    }

    function addStyleToToggleBtn(btn, state) {
        btn.setAttribute("checked", state)
        btn.className = '';
        btn.classList.add('btn');
        btn.classList.add(state ? 'btn-success' : 'btn-danger');
        btn.innerText = state ? 'Enabled' : 'Disabled';
    }

    function initializeToggleBtn(enabled) {
        let btn = document.getElementById('leds-enabled');
        addStyleToToggleBtn(btn, enabled > 0);
        btn.addEventListener("click", function() {
            let checked = this.getAttribute("checked") == 'true';
            let endpoint = checked ? '/disable' : '/enable';
            httpRequest(host + endpoint, 'POST', () => {
                addStyleToToggleBtn(btn, !checked);
            });
        });
    }
    
    function initColorPickers(orders) {
        httpRequest(host + '/getLastStatus', 'GET', function (data) {
            let enabled = data['enabled'];
            initializeToggleBtn(enabled);
            if (/Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.userAgent)) {
                createGUIforMobile(orders, data['ledStrips']);
            } else {
                createGUIforDesktop(orders, data['ledStrips']);
            }
        });

    }
    httpRequest(host + '/getLedStripOrders', 'GET', function (data) {
        initColorPickers(data['orders']);
    });


});
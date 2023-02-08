$(document).ready(function () {
    let host = '';

    function onColorChange(id, colorRGB) {
        let data = colorRGB;
        data['id'] = Number(id);
        $.post(host + '/changeColor', JSON.stringify(data));
    }

    function createGUIforMobile(ids, savedColors) {
        for (let id in ids) {
            let $table = $('<table class="table">'), $thead = $("<thead>"), $tbody = $("<tbody>"),
                $tr = $("<tr>");
            $thead.append($('<th scope="col" class="text-center"> Gate id: ' + id + '</th>'));
            let $td = $("<td>"), $container = $('<div style="display: flex; justify-content:center;">');
            let colors = savedColors.filter(obj => obj.id == id);
            new iro.ColorPicker($container[0], { colors: colors })
                .on('input:end', function (color) { onColorChange(id, color.rgb); });
            $td.append($container);
            $tr.append($td);
            $table.append($thead).append($tbody.append($tr));
            $('#color-pickers-container').append($table);
        }

    }

    function createGUIforDesktop(ids, savedColors) {
        let $table = $('<table class="table">'), $thead = $("<thead>"), $tbody = $("<tbody>"),
            $tr = $("<tr>");
        for (let id in ids) {
            $thead.append($('<th scope="col" class="text-center"> Gate id: ' + id + '</th>'));
            let $td = $("<td>"), $container = $('<div style="display: flex; justify-content:center;">');
            let colors = savedColors.filter(obj => obj.id == id);
            new iro.ColorPicker($container[0], { colors: colors })
                .on('input:end', function (color) { onColorChange(id, color.rgb); });
            $td.append($container);
            $tr.append($td);
        }
        $table.append($thead).append($tbody.append($tr));
        $('#color-pickers-container').append($table);
    }

    function initializeToggleBtn(enabled) {
        let btn = $('#leds-enabled');
        btn.attr("checked", enabled > 0 ? true : false)
        btn.bootstrapToggle({ on: 'Enabled', off: 'Disabled' })
            .change(function () {
                let checked = $(this).prop('checked');
                if (checked) {
                    $.post(host + '/enable');
                } else {
                    $.post(host + '/disable');
                }
            })
    }
    function initColorPickers(ids) {
        $.getJSON(host + '/getLastStatus', function (data) {
            let enabled = data['enabled'];
            initializeToggleBtn(enabled);
            if (/Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.userAgent)) {
                createGUIforMobile(ids, data['ledStrips']);
            } else {
                createGUIforDesktop(ids, data['ledStrips']);
            }
        });

    }
    $.getJSON(host + '/getLedStripIds', function (data) {
        initColorPickers(data['ids']);
    });
});
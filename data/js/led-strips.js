
function onColorChange(id, colorRGB) {
    let data = colorRGB;
    data['id'] = Number(id);
    $.post('/changeColor', JSON.stringify(data));
}

function createGUIforMobile(ids) {
    for(let id in ids) {
        let $table = $('<table class="table">'), $thead = $("<thead>"), $tbody = $("<tbody>"), 
        $tr = $("<tr>");
        $thead.append($('<th scope="col" class="text-center"> Gate id: ' + id + '</th>'));
        let $td = $("<td>"), $container = $('<div style="display: flex; justify-content:center;">');
        new iro.ColorPicker($container[0]).on('input:end', function(color) { onColorChange(id, color.rgb); });
        $td.append($container);
        $tr.append($td);
        $table.append($thead).append($tbody.append($tr));
        $('#color-pickers-container').append($table);
    }

}

function createGUIforDesktop(ids) {
    let $table = $('<table class="table">'), $thead = $("<thead>"), $tbody = $("<tbody>"), 
        $tr = $("<tr>");

    for(let id in ids) {
        $thead.append($('<th scope="col" class="text-center"> Gate id: ' + id + '</th>'));
        let $td = $("<td>"), $container = $('<div style="display: flex; justify-content:center;">');
        new iro.ColorPicker($container[0]).on('input:end', function(color) { onColorChange(id, color.rgb); });
        $td.append($container);
        $tr.append($td);
    }
    $table.append($thead).append($tbody.append($tr));
    $('#color-pickers-container').append($table);
}

function initColorPickers(ids) {
    if( /Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.userAgent) ) {
        createGUIforMobile(ids);
    } else {
        createGUIforDesktop(ids);
    }
}


$(document).ready(function () {
    $.getJSON('/getLedStripIds', function (data) {
        initColorPickers(data['ids']);
    });
});
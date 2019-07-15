function MouseOver(elem, elem2) {
    elem.style.color = "gray";
    getEventData(elem2);
    $('#data').show();
    $(document).bind('mousemove', function (e) {
        $('#data').css({
            hidden: false,
            left: e.pageX + 20,
            top: e.pageY
        });
    });
}

function MouseOut(elem) {
    elem.style.color = "black";
    $('#data').hide();


}

function getEventData(elem2) {

    // call ajax
    var ajax = new XMLHttpRequest();
    var method = "GET";
    var url = "data.php";
    var params = elem2;
    var assynchronous = true;

    ajax.open(method, url + "?name=" + params, assynchronous);
    // sending ajax request
    ajax.send();

    //receive response from data.php
    ajax.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            //converting JSON back to array
            var data = JSON.parse(this.responseText);
            console.log(data);

            // html value
            var html = "";
            var type = data[0];
            var name = data[1];
            var price = data[2];
            var description = data[3];
            var date_of_evidence = data[4];

            html += "typ: " + type + "</br>";
            html += "nazev: " + name + "</br>";
            html += "cena: " + price + "</br>";
            html += "popis: " + description + "</br>";
            html += "datum evidence: " + date_of_evidence + "</br>";

            //add to html
            document.getElementById("data").innerHTML = html;
        }
    }
}

function close_map_modal() {
    document.getElementById("map").style.display = "none";
}

function show_map_modal() {
    document.getElementById("map").style.display = "block";
}
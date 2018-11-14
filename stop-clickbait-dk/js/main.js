(function() {

function main() {
    fetch('data/stop_clickbait_dk.json')
    .then(function (response) {
        return response.json();
    })
    .then(function (data) {
        renderItems(data.posts);
    });
}
window.addEventListener("load", main);


function renderItems(items) {
    items.forEach(renderItem);
}

function renderItem(item, index) {
    var wrapper = $el(".post");

    $el("h1.headline", {
        textContent: item.target.headline
    }).appendTo(wrapper);

    if (item.target.image) {
        var figure = $el("figure").appendTo(wrapper);
        $el("img", {
            src: "data/" + item.target.image.src,
            title: item.target.image.label
        }).appendTo(figure);
        if (item.target.description) {
            $el("figcaption", {
                textContent: item.target.description
            }).appendTo(figure);
        }
    }

    $el("h3", {
        textContent: item.post.message
    }).appendTo(wrapper);

    $el("h5", {
        textContent: "Essensen af historien"
    }).appendTo(wrapper);
    
    $el("p", {
        textContent: item.content,
        style: {
            fontStyle: "italic"
        }
    }).appendTo(wrapper);

    wrapper.appendTo(document.body);

    // console.log({el:wrapper});
}



})();
(function() {

function main() {
    var postNodes = q.all(".userContentWrapper");
    var postData = { posts: [] };
    var imageData = { images: [] };

    var output = document.createElement("textarea");
    output.setAttribute("style", "position: fixed; top: 0; right: 0; width: 50vw; height: 100vh; box-sizing: border-box;");
    document.body.appendChild(output);

    var image_output = document.createElement("div");
    image_output.setAttribute("style", "position: fixed; top: 0; left: 0; width: 50vw; height: 100vh; overflow: auto; background-color: black; color: white; font-family: Arial; box-sizing: border-box;");
    document.body.appendChild(image_output);

    document.body.style.overflow = "hidden";

    postNodes.forEach(function(node) {
        parsePostNode(node, postData.posts, imageData.images, image_output);
    });

    console.log("Posts:", postData.posts.length);

    output.textContent = JSON.stringify(postData);
}

window.addEventListener("load", main);


function parsePostNode(node, nodeArr, imageArr, image_output) {
    var data = {};

    data.content = q("[data-ad-preview=message].userContent", node).textContent;
    data.timestamp = q("abbr[data-utime]", node).getAttribute("title");
    
    if (q("[data-ad-preview=headline]", node).href != "") {
        var storyImage = q(".fbStoryAttachmentImage img", node),
            imageDataURI = storyImage.src!="" ? dataURI(storyImage) : "";

        storyImage.src = imageDataURI;

        data.post = {
            pagename: q(".mts .fwb", node).textContent,
            timestamp: q(".mts abbr[data-utime]", node).getAttribute("title"),
            message: q(".mts [data-ad-preview=message].mtm", node).textContent
        };

        data.target = {
            display: q("[data-ad-preview=display-link]", node).textContent,
            headline: q("[data-ad-preview=headline]", node).textContent,
            url: fb_url(q("[data-ad-preview=headline]", node).href),
            description: q("[data-ad-preview=link-description]", node).textContent
        };

        if (imageDataURI != "") {
            data.target.image = {
                src: "images/image_" + imageArr.length + ".jpg",
                label: storyImage.getAttribute("aria-label")
            };
            imageArr.push(imageDataURI);

            var image_wrapper = document.createElement("div");
            var image_header = document.createElement("a");
            image_header.setAttribute("download", data.target.image.src);
            image_header.setAttribute("href", storyImage.src);
            image_header.appendChild(storyImage);
            image_wrapper.appendChild(image_header);
            image_output.appendChild(image_wrapper);
            image_wrapper.addEventListener("click", function() {
                this.style.opacity = 0.3;
            });
        }

        nodeArr.push(data);
    }
}


var EmptyNode = function() {
    this.textContent = "";
    this.href = "";
    this.src = "";
}
EmptyNode.prototype.getAttribute = function() {
    return "";
}

function q(query, el) {
    var node = (el || document).querySelector(query);
    return node ? node : new EmptyNode();
}
q.all = function (query, el) {
    var nodeList = (el || document).querySelectorAll(query);
    return nodeList.length ? nodeList : [];
}


function fb_url(url) {
    if (!url) return "";
    url = url.substring(url.indexOf("u=")+2, url.indexOf("&"));
    url = url.substring(0, url.indexOf("%3F"));
    url = unescape(url);
    return url;
}


function dataURI(imageNode) {
    var canvas = document.createElement("canvas");
    canvas.width = imageNode.naturalWidth;
    canvas.height = imageNode.naturalHeight;
    canvas.getContext("2d").drawImage(imageNode, 0, 0);
    return canvas.toDataURL('image/jpg');
}


})();
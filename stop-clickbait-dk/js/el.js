(function(global) {

function $el(selector, attributes) {
    // Extract selector parts, and create element
    var tagName = extractRegEx(/^([a-z](?:[a-z0-9]|-)*)/i, selector),
        id = extractRegEx(/#((?:[^.\s])+)/, selector),
        className = extractRegEx(/\.((?:[^.#\s])+)/g, selector),
        el = document.createElement(tagName ? tagName : "div"),
        listeners = {};

    // Apply id and className if applicable
    if (id !== undefined) el.id = id;
    if (className !== undefined) el.className = className;

    // Apply attributes
    Object.keys(attributes || {}).forEach(function(attrName) {
        if (attributes.hasOwnProperty(attrName)) {
            var attrValue = attributes[attrName];
            if (/^((?:inner|outer)(?:HTML|Text)|textContent|nodeValue|className|id)$/.test(attrName)) {
                // Apply directly
                el[attrName] = attrValue;
            }
            else if (attrName=="style" && typeof attrValue == "object") {
                // Style object given
                Object.keys(attrValue).forEach(function(styleName) {
                    if (attrValue.hasOwnProperty(styleName)) {
                        var styleValue = attrValue[styleName];
                        el.style[styleName] = styleValue;
                    }
                });
            }
            else {
                el.setAttribute(attrName, attrValue);
            }
        }
    });

    // Add methods
    el.appendTo = appendTo;
    el.on = function(eventName, handler, useCapture) {
        return addEventListener.call(this, listeners, eventName, handler, useCapture);
    };
    el.off = function(eventName, handler, useCapture) {
        return removeEventListener.call(this, listeners, eventName, handler, useCapture);
    };

    return el;
}

function appendTo(parent) {
    parent.appendChild(this);
    return this;
}

function addEventListener(listeners, eventName, handler, useCapture) {
    this.addEventListener(eventName, handler, useCapture);
    if (listeners[eventName] === undefined) listeners[eventName] = [];
    listeners[eventName].push({handler: handler, useCapture: useCapture});
    return this;
}

function removeEventListener(listeners, eventName, handler, useCapture) {
    // If handler is specified, remove only this
    if (handler !== undefined) this.removeEventListener(eventName, handler, useCapture);
    
    if (listeners[eventName] !== undefined) {
        listeners[eventName] = listeners[eventName].filter(function(listener) {
            if (handler !== undefined) {
                return !(listener.handler === handler && listener.useCapture === useCapture);
            }
            else {
                this.removeEventListener(eventName, listener.handler, listener.useCapture);
                return false;
            }
        }.bind(this));
    }
}

function extractRegEx(regex, str) {
    var result = [],
        match;

    if (typeof str != "string") return;

    do {
        if (match = regex.exec(str))
            result.push(match[1]);
    }
    while (regex.global && match);
    
    return result.length ? result.join(" ") : undefined;
}

global.$el = $el;

})(self);
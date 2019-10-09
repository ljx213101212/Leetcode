//ref:https://github.com/abdulapopoola/Adehun/blob/master/adehun.js
"use strict";

var validStates = {
    PENDING: 0,
    FULFILLED: 1,
    REJECTED: 2
};

var isValidState = function (state) {
    return ((state === validStates.PENDING) ||
            (state === validStates.REJECTED) ||
            (state === validStates.FULFILLED));
};

var Utils = {
    runAsync: function (fn) {
        setTimeout(fn, 0);
    },
    isFunction: function (val) {
        return val && typeof val === "function";
    },
    isObject: function (val) {
        return val && typeof val === "object";
    },
    isPromise: function (val) {
        return val && val.constructor === Adehun;
    }
};

var then = function (onFulfilled) {
    var queuedPromise = new Adehun();
    if (Utils.isFunction(onFulfilled)) {
        queuedPromise.handlers.fulfill = onFulfilled;
    }

    this.queue.push(queuedPromise);
    this.process();

    return queuedPromise;
};

var catchee = function (onRejected) {
    var queuePromise = new Adehun();
    if (Utils.isFunction(onRejected)){
        queuePromise.handlers.reject = onRejected;
    }
    this.queue.push(queuePromise);
    this.process();
    return queuePromise;
}

var transition = function (state, value) {
    if (this.state === state || 
            this.state !== validStates.PENDING ||
            !isValidState(state) ||
            arguments.length !== 2) {
        return;
    }

    this.value = value;
    this.state = state;
    this.process();
};

var process = function () {
    var that = this,
        fulfillFallBack = function (value) {
            return value;
        },
        rejectFallBack = function (reason) {
            throw reason;
        };    
        
    if (this.state === validStates.PENDING) {
        return;
    }

    Utils.runAsync(function () { 
        while (that.queue.length) {
            var queuedPromise = that.queue.shift(),
                handler = null,
                value;

            if (that.state === validStates.FULFILLED) {
                handler = queuedPromise.handlers.fulfill || fulfillFallBack;
            } else if (that.state === validStates.REJECTED) {
                handler = queuedPromise.handlers.reject || rejectFallBack;
            }

            try {
                value = handler(that.value);
            } catch (e) {
                queuedPromise.transition(validStates.REJECTED, e);
                continue;
            }

            Resolve(queuedPromise, value);
        }
    });
};

function Resolve(promise, x) {
    if (promise === x) {
        promise.transition(validStates.REJECTED, new TypeError("The promise and its value refer to the same object"));
    } else if (Utils.isPromise(x)) {
        if (x.state === validStates.PENDING) {
            x.then(function (val) {
                Resolve(promise, val);
            }).catchee(function (reason) {
                promise.transition(validStates.REJECTED, reason);
            });
        } else {
            promise.transition(x.state, x.value);
        }
    } else if (Utils.isObject(x) || Utils.isFunction(x)) {
        var called = false,
            thenHandler;
        try {
            thenHandler = x.then;
            catchHandler = x.catchee;
            
            if (Utils.isFunction(thenHandler)
            && Utils.isFunction(catchHandler)) {
                thenHandler.call(x,
                    function (y) {
                        if (!called) {
                            Resolve(promise, y);
                            called = true;
                        }
                    });
                catchHandler.call(x, function (r) {
                    if (!called) {
                        reject(r);
                        called = true;
                    }
                });
            } else {
                promise.fulfill(x);
                called = true;
            }
        } catch (e) {
            if (!called) {
                promise.reject(e);
                called = true;
            }
        }
    } else {
        promise.fulfill(x);
    }   
}

var fulfill = function (value) {
    this.transition(validStates.FULFILLED, value);
};

var reject = function (reason) {
    this.transition(validStates.REJECTED, reason);
};

var Adehun = function (fn) {
    var that = this;

    this.value = null;
    this.state = validStates.PENDING;
    this.queue = [];
    this.handlers = {
        fulfill : null,
        reject : null
    };

    if (fn) {
        fn(function (value) {
            Resolve(that, value);
        }, function (reason) {
            that.reject(reason);
        });
    }
};

Adehun.prototype.transition = transition;
Adehun.prototype.process = process;
Adehun.prototype.then = then;
Adehun.prototype.catchee = catchee;
Adehun.prototype.fulfill = fulfill;
Adehun.prototype.reject = reject;

module.exports = {
    resolved: function (value) {
        return new Adehun(function (resolve) {
            resolve(value);
        });
    },
    rejected: function (reason) {
        return new Adehun(function (resolve, reject) {
            reject(reason);
        });
    },
    deferred: function () {
        var resolve, reject;

        return {
            promise: new Adehun(function (rslv, rjct) {
                resolve = rslv;
                reject = rjct;
            }),
            resolve: resolve,
            reject: reject
        };
    }
};


// let myPromise = new Adehun((resolve,reject)=>{
//     let input = 1;
//     try{
//         setTimeout(()=>{
//             input +=1;
//             resolve(input);
//         },0);
//     }catch{
//         reject();
//     }
// });


let myPromiseFn = () =>{
    return  new Adehun((resolve,reject)=>{
        let input = 1;
        try{
            setTimeout(()=>{
                input +=1;
                resolve(input);
            },0);
        }catch{
            reject();
        }
    });
}

myPromiseFn().then((output)=>{
    console.log("$$$$$" + String(output));
}).then((output)=>{
    return new Adehun((resolve,reject)=>{
        reject("hahaha");
    });
}).catchee((output)=>{
    console.log(output);
});


let officialPromiseFn = () =>{
    return new Promise((resolve, reject)=>{
        let input = 1;
        try{
            setTimeout(()=>{
                input +=1;
                resolve(input);
            },0);
        }catch{
            reject();
        }
    });
}

officialPromiseFn().then((output) => {
    console.log("@@@@@" + String(output));
})
.catch((err)=>{
    console.log(err);
    return new Promise((resolve,reject)=>{
        reject(123456);
    })
})
.catch((err)=>{
    console.log(err);
});
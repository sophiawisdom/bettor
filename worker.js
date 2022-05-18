let params = {};
let results = [];
let id = null

onmessage = e => {
    if (id != null) {
        clearTimeout(id);
        id = null;
    }
    params = e.data;
    results = [];
    iterations_since_check = 0;

    console.log("updated params!");

    // we do a setTimeout because we might get multiple messages and we want to update to the latest one.
    id = setTimeout(calculate, 0);
}

// When we've done a reasonable amount of calculation, we do a setTimeout of 0ms
// with this function. If there's a message, onmessage() will be called which
// will clear the timeout for this function. So if this function is called it
// means there's no message with new parameters coming and we can
const flushData = () => {
    postMessage({"params": params, "results": results});
    results = [];
    id = null;
    calculate();
}

const calculate = () => {
    iterations_since_check = 0;
    let beginning_bankroll = params.bankroll;
    let increment = params.increment;
    let bet = params.bet;
    let iterations = params.iterations;
    // if i don't add a results.length check here it fails with STATUS_BREAKPOINT,
    // which is fairly mysterious. my guess is it has something to do with too much
    // data being buffered, but not sure.
    while (iterations_since_check < 10_000_000 && results.length < 10000) {
        let bankroll = beginning_bankroll;

        outer:
        for (var i = 0; i < iterations/24; i++) {
            let rand = Math.round(Math.random()*16777216);

            for (let j = 0; j < 24; j++) {
                if (rand&1) {
                    bankroll += bet;
                } else {
                    bankroll -= bet;
                }
                bankroll += increment;
                if (bankroll <= bet) {
                    bankroll = 0;
                    break outer;
                }
                rand >>= 1;
            }
        }

        iterations_since_check += i*24;
        results.push(bankroll);
    }
    id = setTimeout(flushData, 0);
}

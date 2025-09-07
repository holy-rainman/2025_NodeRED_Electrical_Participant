// Get last trigger state from context
let triggered = context.get("triggered") || false;

// Incoming value (12-bit number)
let value = Number(msg.payload);

// Threshold
const THRESHOLD = 300;

if (value < THRESHOLD && !triggered) {
    // First time crossing below threshold → trigger WhatsApp
    triggered = true;
    context.set("triggered", triggered);

    // Send a message (payload can be customized)
    return { payload: "⚠ Alert: Value dropped below " + THRESHOLD + " (" + value + ")" };
}
else if (value >= THRESHOLD && triggered) {
    // Reset when value goes back above threshold
    triggered = false;
    context.set("triggered", triggered);
}

// No output if condition not met
return null;

let timestamp = new Date().toISOString();
let voltage = flow.get("voltage") || 0;
let current = flow.get("current") || 0;
let power = flow.get("power") || 0;
let energy = flow.get("energy") || 0;
let frequency = flow.get("frequency") || 0;
let powerFactor = flow.get("pf") || 0;

// Send as one row (7 columns)
msg.payload = [timestamp, voltage, current, power, energy, frequency, powerFactor];
return msg;

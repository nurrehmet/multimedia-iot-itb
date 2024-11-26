const coap = require('coap');
const fs = require('fs');

const server = coap.createServer();

server.on('request', (req, res) => {
    const serverReceiveTime = Date.now(); // Server receive timestamp

    if (req.method === 'PUT' && req.url === '/upload') {
        let data = Buffer.alloc(0);

        req.on('data', (chunk) => {
            data = Buffer.concat([data, chunk]);
        });

        req.on('end', () => {
            const serverProcessingStartTime = process.hrtime.bigint();

            // Simulate some processing (remove in real-world scenario)
            // const simulatedProcessing = () => {
            //     let sum = 0;
            //     for(let i = 0; i < 1000000; i++) sum += i;
            // };
            // simulatedProcessing();

            const serverProcessingEndTime = process.hrtime.bigint();
            const serverProcessingTimeMs = Number(serverProcessingEndTime - serverProcessingStartTime) / 1000000;

            const payloadString = data.toString();

            // Extract client-side timestamp
            const timestampMatch = payloadString.match(/ClientSendTime: (\d+)/);
            if (!timestampMatch) {
                console.error('No client send timestamp found');
                res.code = '4.00';
                res.end('Invalid payload: No timestamp');
                return;
            }

            const clientSendTime = parseInt(timestampMatch[1]);

            // Calculate actual network latency
            const networkLatency = serverReceiveTime - clientSendTime;
            const throughput = data.length / (serverProcessingTimeMs / 1000);

            console.log(`------ Request Analysis ------`);
            console.log(`Payload Size: ${data.length} bytes`);
            console.log(`Client Send Time: ${clientSendTime} ms`);
            console.log(`Server Receive Time: ${serverReceiveTime} ms`);
            console.log(`Network Latency: ${networkLatency} ms`);
            console.log(`Server Processing Time: ${serverProcessingTimeMs.toFixed(2)} ms`);
            console.log(`Throughput: ${throughput.toFixed(2)} bytes/sec`);

            res.code = '2.04';
            res.end(`Latency: ${networkLatency} ms\nProcessing Time: ${serverProcessingTimeMs.toFixed(2)} ms\nThroughput: ${throughput.toFixed(2)} bytes/sec`);
        });
    } else {
        res.code = '4.04';
        res.end('Not Found');
    }
});

const PORT = 8080;
server.listen(PORT, () => {
    console.log(`CoAP server listening on port ${PORT}`);
});

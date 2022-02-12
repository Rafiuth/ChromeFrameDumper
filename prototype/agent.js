/// <reference path="frida-gum.d.ts" />

let chromeMod = Module.load("chrome.dll");
let chromeBase = chromeMod.base;

console.log("Chrome Module: " + chromeMod);

//0x04AFD270 media::DecryptingDemuxerStream::OnBufferDecrypted
//0x05190D50 media::MojoDecryptor::Decrypt
//0x0082D420 media::DecoderStream<media::DemuxerStream::AUDIO>::OnBufferReady
Interceptor.attach(chromeBase.add(0x0082D420), {
    onEnter() {
        //let callStack = Thread.backtrace(this.context, Backtracer.ACCURATE);
        //console.log(`CdmDecrypt`, callStack.map(v => v.sub(chromeMod)));

        //rcx, rdx, r8, r9, [stack]
        let demuxerStream = this.context.rcx;
        let status = this.context.rdx;
        let buffer = this.context.r8;

        let stream = demuxerStream.add(80).readPointer();

        console.log("OnBufferReady", demuxerStream, status, buffer);
    }
});

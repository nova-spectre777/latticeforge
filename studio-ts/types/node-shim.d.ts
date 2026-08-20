declare module 'node:test' { export function test(name:string, fn:()=>void):void; }
declare module 'node:assert/strict' { export function equal(a:unknown,b:unknown):void; export function deepEqual(a:unknown,b:unknown):void; }

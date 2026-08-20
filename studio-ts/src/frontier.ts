export type Candidate={securityBits:number;failureLog2:number;publicKeyBytes:number;ciphertextBytes:number;id:string};
export function dominates(a:Candidate,b:Candidate):boolean{
 const noWorse=a.securityBits>=b.securityBits&&a.failureLog2<=b.failureLog2&&a.publicKeyBytes<=b.publicKeyBytes&&a.ciphertextBytes<=b.ciphertextBytes;
 const strict=a.securityBits>b.securityBits||a.failureLog2<b.failureLog2||a.publicKeyBytes<b.publicKeyBytes||a.ciphertextBytes<b.ciphertextBytes;
 return noWorse&&strict;
}
export function pareto(xs:Candidate[]):Candidate[]{return xs.filter((x,i)=>!xs.some((y,j)=>i!==j&&dominates(y,x)));}
export function normalizedSecurity(c:Candidate,target:number):number{return Math.min(1,c.securityBits/Math.max(1,target));}

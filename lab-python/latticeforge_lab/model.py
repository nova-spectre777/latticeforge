from __future__ import annotations
from dataclasses import dataclass, asdict
import hashlib, json, math

@dataclass(frozen=True)
class Params:
    n:int; q:int; k:int; sigma:float; eta:int

def domain_ok(p:Params)->bool:
    return 128 <= p.n <= 16384 and p.q > 2 and 1 <= p.k <= 16 and 0 < p.sigma < p.q/2 and 1 <= p.eta <= 32

def failure_log2(p:Params)->float:
    if not domain_ok(p): return 0.0
    sigma_eff=p.sigma*math.sqrt(p.k*p.eta*p.n/8.0)
    margin=p.q/4.0
    ln_prob=math.log(2.0)-(margin*margin)/(2*sigma_eff*sigma_eff)
    return min(0.0, ln_prob/math.log(2.0))

def attacks(p:Params)->dict[str,float]:
    if not domain_ok(p): return {}
    dim=p.n*p.k; hardness=max(1.0, math.log2(p.q/p.sigma)); ep=math.log2(p.eta+1)
    hg=min(64.0,dim/16.0)
    return {
      'primal-reference':max(0.0,.205*dim+5*hardness-2*ep),
      'dual-reference':max(0.0,.190*dim+6*hardness+.8*p.k-1.5*ep),
      'hybrid-reference':max(0.0,.175*(dim-hg)+.55*hg+4.5*hardness-ep),
    }

def certificate(p:Params, security_target:float=128, failure_target:float=-96)->dict:
    a=attacks(p)
    if not a:
        status='REJECTED'; cheapest=None; sec=0.0
    else:
        cheapest=min(a,key=a.get); sec=a[cheapest]
        status='VALID_UNDER_DECLARED_MODEL' if sec>=security_target and failure_log2(p)<=failure_target else 'REJECTED'
    body={'algorithm_version':'SPECTRA-Ω/v0.1','status':status,'candidate':asdict(p),'models':a,'constraints':{'security_bits':security_target,'max_failure_log2':failure_target},'counterexample':None if status.startswith('VALID') else {'cheapest_attack':cheapest,'cost_bits':sec}}
    canonical=json.dumps(body,sort_keys=True,separators=(',',':')).encode()
    body['sha256']=hashlib.sha256(canonical).hexdigest()
    return body

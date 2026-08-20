from .model import Params, certificate

def run():
    tested=valid=counterexamples=0
    best=None
    for n in (256,512,768,1024):
      for q in (3329,7681,12289):
       for k in (2,3,4):
        for sigma in (2.8,3.2,3.6):
         for eta in (2,3):
          tested+=1; c=certificate(Params(n,q,k,sigma,eta),96,-32)
          if c['status'].startswith('VALID'):
            valid+=1
            score=min(c['models'].values())
            if best is None or score>best[0]: best=(score,c)
          else: counterexamples+=1
    return {'tested':tested,'valid':valid,'rejected':counterexamples,'best':best[1] if best else None}

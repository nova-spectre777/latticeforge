import unittest
from latticeforge_lab.model import Params, domain_ok, attacks, certificate
from latticeforge_lab.benchmark import run
class T(unittest.TestCase):
 def test_domain(self): self.assertFalse(domain_ok(Params(0,2,0,0,0)))
 def test_counterexample(self):
  c=certificate(Params(256,3329,2,3.2,2),300,-1)
  self.assertEqual(c['status'],'REJECTED'); self.assertIsNotNone(c['counterexample'])
 def test_certificate_stable(self):
  p=Params(1024,12289,4,3.2,2)
  self.assertEqual(certificate(p,80,-1)['sha256'],certificate(p,80,-1)['sha256'])
 def test_benchmark(self): self.assertEqual(run()['tested'],216)
if __name__=='__main__': unittest.main()

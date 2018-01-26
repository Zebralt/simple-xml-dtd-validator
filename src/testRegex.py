# Jocelyn Vernay
# M2 Data & Knowledge
# Web Data Models - Project : DTD Validator for XML
# October 17th, 2017

import validator as vd

if __name__ == "__main__":
    vd.loadTests("../tests/test_brackets")
    vd.loadTests("../tests/test_loop")
    vd.loadTests("../tests/test_skip")
    vd.loadTests("../tests/test_loopXskip.txt")
    vd.loadTests("../tests/test_combined")
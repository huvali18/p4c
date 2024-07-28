import os, subprocess

fp = os.path.join(os.path.abspath(os.path.dirname(__file__)), "p4_multi/firewall/firewall.p4")
preprocessed = subprocess.check_output(
    "gcc -x c++ -E -P %s" % fp, shell=True).decode('utf-8')
print(preprocessed)
fmtp = "%s.pp.formatted" % fp
with open(fmtp, 'w+') as f:
    f.write(preprocessed)
os.system("clang-format -i %s" % fmtp)
with open(fmtp) as f:
    preprocessed = f.read()
os.remove(fmtp)

lines = 2
for l in preprocessed.splitlines():
        if len(l.strip()) > 0:
            lines = lines + 1

print(lines)
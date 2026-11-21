#!/bin/bash
# Crea una rama dev/<nombre> por integrante y las sube. Ejecutar UNA vez (Cristopher).
set -e
for n in cristopher mateo martin sebastian jhon luis limaico ariel; do
  git branch "dev/$n" 2>/dev/null || echo "dev/$n ya existe"
done
git push -u origin --all
echo "Ramas listas. Cada quien: git fetch && git checkout dev/<su-nombre>"

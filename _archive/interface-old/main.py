import streamlit as st
import json
import pandas as pd 

import os, sys, json
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from database.db import *

st.write("""
### Proxima
""")

puzzles = []
for puzzle in Puzzle.select().dicts():
    puzzle['state'] = json.loads(puzzle['state'])
    puzzles.append(puzzle)
    
st.write(puzzles)
df = pd.DataFrame(puzzles)
st.write(df)
puzzle_df = df.drop('state', axis=1)


table_data=[]
for item in puzzles:
    if item["name"] == "RCP":
        continue
    registers = item['state']['registers']
    row = {"Name": item["name"], "Last Update": item["updated_at"]}
    for i, reg in enumerate(registers):
        row[f"Reg{i}"] = reg
    table_data.append(row)

if table_data:
    df_reg = pd.DataFrame(table_data)
    # st.table(df_reg)
else:
    st.write("No data to display.")

for name in df_reg['Name'].unique():
    st.write(f"Registers for {name}:")
    filtered_df = df_reg[df_reg['Name'] == name].reset_index(drop=True)
    st.table(filtered_df.style.set_table_styles([{'selector': 'th', 'props': [('background', '#3C6E71'), ('color', 'white'), ('font-weight', 'bold')]}, {'selector': 'td', 'props': [('border', '1px solid #3C6E71')]}]))

import tkinter as tk
from tkinter import ttk, filedialog, messagebox


def display_results_in_table(file_path1, file_path2):
    # Main Display
    root = tk.Tk()
    root.title("Threads Results")
    root.geometry("1400x600")

    notebook = ttk.Notebook(root)
    notebook.pack(expand=1, fill="both")

    
    # commands.txt
    frame1 = ttk.Frame(notebook)
    notebook.add(frame1)
    
    commands_label = ttk.Label(frame1, text="Concurrent Hash Table", font=('Arial', 12, 'bold'))
    commands_label.pack(pady=10)

    commands_tree = ttk.Treeview(frame1, columns=("Commands"), show='headings')
    commands_tree.pack(side=tk.LEFT, expand=1, fill='both')
    commands_tree.heading("Commands", text="Commands")
    
    
    with open(file_path1, 'r') as file:
        for line in file:
            commands_tree.insert('', 'end', values=(line.strip(),))

    # output.txt
    output_tree = ttk.Treeview(frame1, columns=("Output"), show='headings')
    output_tree.pack(side=tk.LEFT, expand=1, fill='both')
    output_tree.heading("Output", text="Output")
    
    
    with open(file_path2, 'r') as file:
        for line in file:
            output_tree.insert('', 'end', values=(line.strip(),))
        
    root.mainloop()


def main():
    display_results_in_table("commands.txt", "output.txt")

if __name__ == "__main__":
    main()
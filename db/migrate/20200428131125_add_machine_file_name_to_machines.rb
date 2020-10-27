class AddMachineFileNameToMachines < ActiveRecord::Migration[5.0]
  def change
    add_column :machines, :machine_file_name, :string
  end
end

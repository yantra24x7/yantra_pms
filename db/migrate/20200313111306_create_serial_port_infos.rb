class CreateSerialPortInfos < ActiveRecord::Migration[5.0]
  def change
    create_table :serial_port_infos do |t|
      t.string :port
      t.integer :baurd_rate
      t.integer :data_bit
      t.integer :stop_bit
      t.integer :parity
      t.string :flow_control
      t.references :machine, foreign_key: true

      t.timestamps
    end
  end
end
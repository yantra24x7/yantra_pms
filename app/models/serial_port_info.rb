class SerialPortInfo < ApplicationRecord
  belongs_to :machine
  enum parity: {"Odd": 1, "Even": 2}
  validates :machine_id, presence: true, uniqueness: true
  # enum parity: [:Odd, :Even]
end

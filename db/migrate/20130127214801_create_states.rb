class CreateStates < ActiveRecord::Migration
  def up
    create_table :states do |t|
      t.string :SYSSW
      t.string :BARSW
      t.string :EXTSW
      t.string :RCCSW
      t.string :WINSW
      t.integer :BATR
      t.integer :BATU
      t.integer :BATW
      t.integer :BAT5
      t.integer :BAT4
      t.integer :BAT3
      t.integer :BAT2
      t.integer :BAT1
      
      t.timestamps
    end
  end

  def down
    drop_table :states
  end
end

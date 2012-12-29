class CreateMeasurements < ActiveRecord::Migration
  def up
    create_table :measurements do |t|
      t.integer :DT
      t.float :T0
      t.integer :H0
      t.float :T1
      t.integer :H1
      t.float :T2
      t.integer :H2
      t.float :T3
      t.integer :H3
      t.float :T4
      t.integer :H4
      t.float :T5
      t.integer :H5
      t.float :PRESS
      t.integer :UV
      t.integer :FC
      t.integer :STORM
      t.integer :WD
      t.float :WS
      t.float :WG
      t.float :WC
      t.integer :RC
      
    end
  end

  def down
    drop_table :measurements
  end
end
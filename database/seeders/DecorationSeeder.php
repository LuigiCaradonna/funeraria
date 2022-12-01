<?php

namespace Database\Seeders;

use Illuminate\Database\Seeder;
use Illuminate\Support\Facades\DB;

class DecorationSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        $decoration = [
            ['name' => 'Angelo'],
            ['name' => 'Corona'],
            ['name' => 'Farfalla'],
            ['name' => 'Cuore'],
            ['name' => 'Chiave di violino'],
            ['name' => 'Bici']
        ];

        DB::table('decoration')->insert($decoration);
    }
}
